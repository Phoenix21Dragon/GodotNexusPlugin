#include "NexusNode.h"
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/packed_int32_array.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/file_access.hpp>

#include "nexus.h"
#include "vcg/space/point3.h"
#include "vcg/space/point2.h"

#include <map>
#include <thread>

using namespace godot;

void NexusNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_node_loaded", "mesh"), &NexusNode::_on_node_loaded);
	ClassDB::bind_method(D_METHOD("openNexusModell", "url"), &NexusNode::openNexusModell);
	ClassDB::bind_method(D_METHOD("start_loading"), &NexusNode::start_loading);
	ClassDB::bind_method(D_METHOD("loadNexusNode", "node_index"), &NexusNode::loadNexusNode);	
	ClassDB::bind_method(D_METHOD("loadNodesAsync"), &NexusNode::loadNodesAsync);
	ADD_SIGNAL(MethodInfo("node_loaded", PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "ArrayMesh")));
}

NexusNode::NexusNode() {
	// Initialize any variables here.
	main_thread_id =  std::this_thread::get_id();
}

NexusNode::~NexusNode() {
	// Add your cleanup here.
}

void NexusNode::_process(double delta) {
	
}

void NexusNode::_on_node_loaded(Ref<ArrayMesh> mesh) {
	
	UtilityFunctions::print("\nNexusNode.cpp: _on_node_loaded()");
	UtilityFunctions::print("VALID mesh? ", mesh.is_valid());

	if (std::this_thread::get_id() == main_thread_id) {
		UtilityFunctions::print("Läuft im Main-Thread!");
	} else {
		UtilityFunctions::print("NICHT im Main-Thread!");
	}

	if (mesh.is_null()) {
		UtilityFunctions::printerr("MeshInstance konnte nicht geladen werden");
		return;
	}

	set_mesh(mesh);	
}

bool NexusNode::openNexusModell(String url) {
	url = url.replace("res:/", ".");
	UtilityFunctions::print("function openNexusModell: url=", url);
	
	this->nexus = new nx::Nexus();
	bool success = this->nexus->open(url.utf8().get_data());
	return success;
}

void NexusNode::start_loading() {
	this->connect("node_loaded", Callable(this, "_on_node_loaded"));

	worker_thread.instantiate();
	worker_thread->start(Callable(this, "loadNodesAsync"), Thread::Priority::PRIORITY_HIGH);
}

Ref<ArrayMesh> NexusNode::loadNexusNode(int node_index) {

	UtilityFunctions::print("LOADING of Node: ", node_index);

	nexus->loadRam(node_index);
	// UtilityFunctions::print("\nNode Info:");
	// UtilityFunctions::print("    node[", node_index, "].first_patch:  ", nexus->nodes[node_index].first_patch);
	// UtilityFunctions::print("    node[", node_index, "].last_patch(): ", nexus->nodes[node_index].last_patch());
	// UtilityFunctions::print("    node[", node_index, "].offset:       ", nexus->nodes[node_index].offset);	
	// UtilityFunctions::print("    node[", node_index, "].nvert:        ", nexus->nodes[node_index].nvert);
	// UtilityFunctions::print("    node[", node_index, "].nface:        ", nexus->nodes[node_index].nface);
	// UtilityFunctions::print("");	

	Ref<ArrayMesh> mesh;
	mesh.instantiate();
	mesh->set_name("node_" + node_index);

	nx::Node& node = nexus->nodes[node_index];
	nx::NodeData& data = nexus->nodedata[node_index];
	nx::Signature& sig = nexus->header.signature;

	uint32_t nvert = node.nvert;
	uint16_t* indices = data.faces(sig, nvert);
	vcg::Point3f* coords = data.coords();
	vcg::Point2f* uvs = data.texCoords(sig, nvert);
	vcg::Point3s* normals = data.normals(sig, nvert);


	// UtilityFunctions::print("\nLOAD GEOMETRY");	
	uint32_t offset = node.offset;
	for (uint32_t k = node.first_patch; k < node.last_patch(); k++) {
		nx::Patch& patch = nexus->patches[k];
		uint32_t tex_index = patch.texture;
		uint32_t next_offset = patch.triangle_offset;
		uint32_t face_count = next_offset - offset;

		// UtilityFunctions::print("Patch ", k, ": offset=", offset, ", next_offset=", next_offset, ", face_count=", face_count);

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
		// UtilityFunctions::print("    surface_index: ", surface_index);

		// UtilityFunctions::print("    tex_index: ", tex_index);
		// UtilityFunctions::print("    tex_index != 0xffffffff: ", (tex_index != 0xffffffff));
		// UtilityFunctions::print("    tex_index < nexus->header.n_textures: ", (tex_index < nexus->header.n_textures));

		// UtilityFunctions::print("\n    LOAD TEXTURES");
		// Schritt 2: Textur laden und Material zuweisen
		if (tex_index != 0xffffffff && tex_index < nexus->header.n_textures) {
				
			nx::TextureData& tex_data = nexus->texturedata[tex_index];
			
			// UtilityFunctions::print("    tex_data.width: ", tex_data.width);
			// UtilityFunctions::print("    tex_data.height: ", tex_data.height);
			
			if (!tex_data.memory) {
				UtilityFunctions::printerr("Tex index ", tex_index, " hat kein gültiges Speicherabbild (memory=nullptr).");
				continue;
			}
			uint64_t begin = nexus->textures[tex_index].getBeginOffset();
			uint64_t end = nexus->textures[tex_index].getEndOffset();
			int tex_size = end - begin;
			// int tex_size = tex_data.width * tex_data.height * 4; 
			// UtilityFunctions::print("    tex_size: ", tex_size);
			
			PackedByteArray byte_array;
			byte_array.resize(tex_size); // Verwende tatsächliche JPEG-Größe
			memcpy(byte_array.ptrw(), tex_data.memory, tex_size);
			
			Ref<Image> image;
			image.instantiate();

			Error err = image->load_jpg_from_buffer(byte_array);
			if (err != OK || image->is_empty()) {
				UtilityFunctions::printerr("Fehler beim Laden des JPEG aus Speicher für tex_index: ", tex_index);
			}
			// UtilityFunctions::print("    Image Format: ", image->get_format());
			// UtilityFunctions::print("    image_jpg->get_width(): ", image->get_width());
			// UtilityFunctions::print("    image_jpg->get_height(): ", image->get_height());

			// image->save_jpg("res://nexus_texture_" + itos(tex_index) + "_from_image.jpg");

			// image = image->load_from_file("res://nexus_texture_" + itos(tex_index) + "_from_image.jpg");

			Ref<ImageTexture> texture;
			// texture.instantiate();
			texture = ImageTexture::create_from_image(image);

			// UtilityFunctions::print("    Image Format: ", texture->get_format());
			// UtilityFunctions::print("    texture->get_width(): ", texture->get_width());
			// UtilityFunctions::print("    texture->get_height(): ", texture->get_height());
			// UtilityFunctions::print("");

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
	
	// UtilityFunctions::print("NexusNode.cpp: Calling Deferred");
	// call_deferred( "_on_node_loaded", mesh);

	// UtilityFunctions::print("NexusNode.cpp: Calling Signal");
	// call_deferred("emit_signal", "node_loaded", mesh);

	// UtilityFunctions::print("VALID mesh? ", mesh.is_valid());

	return mesh;
}

void NexusNode::loadNodesAsync() {
	for (size_t i = 1; i < 10; i++)
	{
		Ref<ArrayMesh> mesh = loadNexusNode(i);

		String save_path = "res://mesh_node_" + itos(i) + ".tres";
		Error save_err = ResourceSaver::get_singleton()->save(mesh, save_path);
		if (save_err != OK) {
			UtilityFunctions::printerr("Fehler beim Speichern des Mesh als .tres: ", save_err);
		} else {
			UtilityFunctions::print("Mesh erfolgreich als .tres gespeichert: ", save_path);
		}
	}	
}