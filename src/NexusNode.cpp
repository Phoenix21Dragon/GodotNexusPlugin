#include "NexusNode.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/packed_int32_array.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include "vcg/space/point3.h"
#include "vcg/space/point2.h"

#include "nexus.h"
#include <map>

using namespace godot;

void NexusNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("loadNexusNode", "node_index", "options"), &NexusNode::loadNexusNode);
	ClassDB::bind_method(D_METHOD("openNexusModell", "url"), &NexusNode::openNexusModell);
}

NexusNode::NexusNode() {
	// Initialize any variables here.
}

NexusNode::~NexusNode() {
	// Add your cleanup here.
}

void NexusNode::_process(double delta) {
	
}

bool NexusNode::openNexusModell(String url) {
	url = url.replace("res:/", ".");
	UtilityFunctions::print("function openNexusModell: url=", url);
	
	this->nexus = new nx::Nexus();
	bool success = this->nexus->open(url.utf8().get_data());
	return success;
}

Ref<Image> recolor_image_with_patch_index(Ref<Image> img, int patch) {
    // Sicherheitscheck
    if (img.is_null() || img->is_empty()) {
        return img;
    }

    int width = img->get_width();
    int height = img->get_height();

    
    float r_mod = (((patch * 97)  % 255)) / 255.0f; 
    float g_mod = (((patch * 181) % 255)) / 255.0f;
    float b_mod = (((patch * 43)  % 255)) / 255.0f;

    // // Zusätzliche Variation mit Sinus (macht fließendere Übergänge)
    // r_mod = fmod(r_mod + 0.5f * (sinf(patch * 0.7f) + 1.0f) / 2.0f, 1.0f);
    // g_mod = fmod(g_mod + 0.5f * (sinf(patch * 1.3f) + 1.0f) / 2.0f, 1.0f);
    // b_mod = fmod(b_mod + 0.5f * (sinf(patch * 2.1f) + 1.0f) / 2.0f, 1.0f);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Color col = img->get_pixel(x, y);

            // Mischung 50% Original, 50% Node-Farbe
            col.r = col.r * 0.5f + r_mod * 0.5f;
            col.g = col.g * 0.5f + g_mod * 0.5f;
            col.b = col.b * 0.5f + b_mod * 0.5f;

            img->set_pixel(x, y, col);
        }
    }

    return img;
}

Ref<ArrayMesh> NexusNode::loadNexusNode(int node_index, Dictionary options) {
	UtilityFunctions::print("LOADING of Node: ", node_index);

	nexus->loadRam(node_index);
	UtilityFunctions::print("\nNode Info:");
	UtilityFunctions::print("    node[", node_index, "].first_patch:  ", nexus->nodes[node_index].first_patch);
	UtilityFunctions::print("    node[", node_index, "].last_patch(): ", nexus->nodes[node_index].last_patch());
	UtilityFunctions::print("    node[", node_index, "].offset:       ", nexus->nodes[node_index].offset);	
	UtilityFunctions::print("    node[", node_index, "].nvert:        ", nexus->nodes[node_index].nvert);
	UtilityFunctions::print("    node[", node_index, "].nface:        ", nexus->nodes[node_index].nface);
	UtilityFunctions::print("");	

	Ref<ArrayMesh> mesh;
	mesh.instantiate();

	nx::Node& node = nexus->nodes[node_index];
	nx::NodeData& data = nexus->nodedata[node_index];
	nx::Signature& sig = nexus->header.signature;

	uint32_t nvert = node.nvert;
	uint16_t* indices = data.faces(sig, nvert);
	vcg::Point3f* coords = data.coords();
	vcg::Point2f* uvs = data.texCoords(sig, nvert);
	vcg::Point3s* normals = data.normals(sig, nvert);


	UtilityFunctions::print("\nLOAD GEOMETRY");	
	uint32_t offset = 0;
	for (uint32_t k = node.first_patch; k < node.last_patch(); k++) {
		nx::Patch& patch = nexus->patches[k];
		uint32_t tex_index = patch.texture;
		uint32_t next_offset = patch.triangle_offset;
		uint32_t face_count = next_offset - offset;

		UtilityFunctions::print("Patch ", k, ": offset=", offset, ", next_offset=", next_offset, ", face_count=", face_count);

		// if (face_count == 0) {
		// 	continue; // Überspringen
		// }

		PackedVector3Array godot_vertices;
		PackedVector3Array godot_normals;
		PackedVector2Array godot_uvs;
		PackedInt32Array godot_indices;

		std::map<uint16_t, int> index_map;
		int local_index = 0;


		// Nur betroffene Vertices und Faces sammeln
		for (uint32_t f = offset; f < next_offset; ++f) {
			for (int v = 0; v < 3; ++v) {
				uint16_t global_idx = indices[f * 3 + v];

				if (index_map.find(global_idx) == index_map.end()) {
					Vector3 pos = Vector3(coords[global_idx].X(), coords[global_idx].Y(), coords[global_idx].Z());
					godot_vertices.push_back(pos);

					if (normals)
						godot_normals.push_back(Vector3(normals[global_idx].X(), normals[global_idx].Y(), normals[global_idx].Z()));
					if (uvs)
						godot_uvs.push_back(Vector2(uvs[global_idx].X(), uvs[global_idx].Y()));

					index_map[global_idx] = local_index++;
				}
				godot_indices.push_back(index_map[global_idx]);
			}
		}

		Array arrays;
		arrays.resize(Mesh::ARRAY_MAX);
		arrays[Mesh::ARRAY_VERTEX] = godot_vertices;
		arrays[Mesh::ARRAY_NORMAL] = godot_normals;
		arrays[Mesh::ARRAY_TEX_UV] = godot_uvs;
		arrays[Mesh::ARRAY_INDEX] = godot_indices;

		int surface_index = mesh->get_surface_count();
		mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);
		mesh->surface_set_name(surface_index, "patch " + itos(k) + " with child " + itos(patch.node));
		UtilityFunctions::print("    surface_index: ", surface_index);

		UtilityFunctions::print("    tex_index: ", tex_index);
		UtilityFunctions::print("    tex_index != 0xffffffff: ", (tex_index != 0xffffffff));
		UtilityFunctions::print("    tex_index < nexus->header.n_textures: ", (tex_index < nexus->header.n_textures));

		UtilityFunctions::print("\n    LOAD TEXTURES");
		// Schritt 2: Textur laden und Material zuweisen
		if (tex_index != 0xffffffff && tex_index < nexus->header.n_textures) {
				
			nx::TextureData& tex_data = nexus->texturedata[tex_index];
			
			UtilityFunctions::print("    tex_data.width: ", tex_data.width);
			UtilityFunctions::print("    tex_data.height: ", tex_data.height);
			
			if (!tex_data.memory) {
				UtilityFunctions::printerr("Tex index ", tex_index, " hat kein gültiges Speicherabbild (memory=nullptr).");
				continue;
			}
			uint64_t begin = nexus->textures[tex_index].getBeginOffset();
			uint64_t end = nexus->textures[tex_index].getEndOffset();
			int tex_size = end - begin;
			// int tex_size = tex_data.width * tex_data.height * 4; 
			UtilityFunctions::print("    tex_size: ", tex_size);
			
			PackedByteArray byte_array;
			byte_array.resize(tex_size); // Verwende tatsächliche JPEG-Größe
			memcpy(byte_array.ptrw(), tex_data.memory, tex_size);
			
			Ref<Image> image;
			image.instantiate();

			Error err = image->load_jpg_from_buffer(byte_array);
			if (err != OK || image->is_empty()) {
				UtilityFunctions::printerr("Fehler beim Laden des JPEG aus Speicher für tex_index: ", tex_index);
			}
			UtilityFunctions::print("    Image Format: ", image->get_format());
			UtilityFunctions::print("    image_jpg->get_width(): ", image->get_width());
			UtilityFunctions::print("    image_jpg->get_height(): ", image->get_height());

			// image = image->load_from_file("res://nexus_texture_" + itos(tex_index) + "_from_image.jpg");
			// image->save_jpg("res://nexus_texture_" + itos(tex_index) + "_from_image.jpg");

			if (options.get("colored_patches", false))
			{
				image = recolor_image_with_patch_index(image, k);
			}			

			Ref<ImageTexture> texture;
			// texture.instantiate();
			texture = ImageTexture::create_from_image(image);

			UtilityFunctions::print("    Image Format: ", texture->get_format());
			UtilityFunctions::print("    texture->get_width(): ", texture->get_width());
			UtilityFunctions::print("    texture->get_height(): ", texture->get_height());
			UtilityFunctions::print("");

			// Material erstellen
			Ref<StandardMaterial3D> material;
			material.instantiate();
			material->set_texture(StandardMaterial3D::TEXTURE_ALBEDO, texture);
			material->set_cull_mode(BaseMaterial3D::CullMode::CULL_DISABLED);
			material->set_uv1_scale(Vector3(1.0, -1.0, 1.0));
			material->set_shading_mode(BaseMaterial3D::ShadingMode::SHADING_MODE_UNSHADED);
			mesh->surface_set_material(surface_index, material);
		}

		offset = next_offset;
	}

	return mesh;
}

