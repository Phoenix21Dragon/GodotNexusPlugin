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
	ClassDB::bind_method(D_METHOD("loadNexusModell", "url"), &NexusNode::loadNexusModell);
}

NexusNode::NexusNode() {
	// Initialize any variables here.
}

NexusNode::~NexusNode() {
	// Add your cleanup here.
}

void NexusNode::_process(double delta) {
	
}

// Ref<ArrayMesh> NexusNode::loadNexusModell(String url){

// 	url = url.replace("res:/", ".");
	
// 	nx::Nexus* nexus = new nx::Nexus();
// 	nexus->filename = std::string(url.utf8().get_data());
// 	nexus->file->setFileName(url.utf8().get_data());
// 	nexus->file->open(nx::NexusFile::Read);
// 	nexus->loadHeader();
// 	UtilityFunctions::print("magic: ", nexus->header.magic);
// 	UtilityFunctions::print("version: ", nexus->header.version);
// 	UtilityFunctions::print("nvert: ", nexus->header.nvert);
// 	UtilityFunctions::print("nface: ", nexus->header.nface);
// 	UtilityFunctions::print("n_nodes: ", nexus->header.n_nodes);
// 	UtilityFunctions::print("n_patches: ", nexus->header.n_patches);
// 	UtilityFunctions::print("n_textures: ", nexus->header.n_textures);
	
// 	nexus->loadIndex();
// 	// UtilityFunctions::print("nexus->nodes[0].offset: ", nexus->nodes[0].offset);
// 	// UtilityFunctions::print("nexus->nodes[0].getBeginOffset: ", nexus->nodes[0].getBeginOffset());
// 	// UtilityFunctions::print("nexus->nodes[0].getEndOffset: ", nexus->nodes[0].getEndOffset());
// 	// UtilityFunctions::print("nexus->nodes[0].getSize: ", nexus->nodes[0].getSize());

// 	// UtilityFunctions::print("nexus->nodes[1].offset: ", nexus->nodes[1].offset);
// 	// UtilityFunctions::print("nexus->nodes[1].getBeginOffset: ", nexus->nodes[1].getBeginOffset());
// 	// UtilityFunctions::print("nexus->nodes[1].getEndOffset: ", nexus->nodes[1].getEndOffset());
// 	// UtilityFunctions::print("nexus->nodes[1].getSize: ", nexus->nodes[1].getSize());
	
// 	// UtilityFunctions::print("nexus->nodes[2].offset: ", nexus->nodes[2].offset);
// 	// UtilityFunctions::print("nexus->nodes[2].getBeginOffset: ", nexus->nodes[2].getBeginOffset());
// 	// UtilityFunctions::print("nexus->nodes[2].getEndOffset: ", nexus->nodes[2].getEndOffset());
// 	// UtilityFunctions::print("nexus->nodes[2].getSize: ", nexus->nodes[2].getSize());
	
// 	// UtilityFunctions::print("nexus->patches->texture: ", nexus->patches->texture);
// 	// UtilityFunctions::print("nexus->textures->offset: ", nexus->textures->offset);

// 	nexus->loadRam(0);

// 	nx::NodeData& node_data = nexus->nodedata[0];
// 	nx::Node& node = nexus->nodes[0];

// 	uint32_t nvert = node.nvert;
// 	uint32_t nface = node.nface;

// 	vcg::Point3f* coords = node_data.coords();
// 	vcg::Point2f* uvs = node_data.texCoords(nexus->header.signature, nvert);
// 	vcg::Point3s* normals = node_data.normals(nexus->header.signature, nvert);
// 	uint16_t* indices = node_data.faces(nexus->header.signature, nvert);

// 	PackedVector3Array godot_vertices;
// 	PackedVector3Array godot_normals;
// 	PackedVector2Array godot_uvs;
// 	PackedInt32Array godot_indices;

// 	UtilityFunctions::print("coords[0].X(): ", coords[0].X());
// 	UtilityFunctions::print("coords[1].X(): ", coords[1].X());
// 	UtilityFunctions::print("coords[2].X(): ", coords[2].X());

// 	for (uint32_t i = 0; i < nvert; ++i) {
// 		godot_vertices.push_back(Vector3(coords[i].X(), coords[i].Y(), coords[i].Z()));
// 		if (normals)
// 			godot_normals.push_back(Vector3(normals[i].X(), normals[i].Y(), normals[i].Z()));
// 		if (uvs)
// 			godot_uvs.push_back(Vector2(uvs[i].X(), uvs[i].Y()));
// 	}

// 	for (uint32_t i = 0; i < nface * 3; ++i) {
// 		godot_indices.push_back(indices[i]);
// 	}

// 	Array arrays;
// 	arrays.resize(Mesh::ARRAY_MAX);
// 	arrays[Mesh::ARRAY_VERTEX] = godot_vertices;
// 	arrays[Mesh::ARRAY_NORMAL] = godot_normals;
// 	arrays[Mesh::ARRAY_TEX_UV] = godot_uvs;
// 	arrays[Mesh::ARRAY_INDEX] = godot_indices;

// 	Ref<ArrayMesh> mesh;
// 	mesh.instantiate();
// 	UtilityFunctions::print("IS VALID: ", mesh.is_valid());
//  	mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);

// 	// === TEXTUR LADEN UND ANWENDEN ===
// 	if (nexus->header.n_textures > 0 && nexus->patches && nexus->patches->texture < nexus->header.n_textures) {
// 		UtilityFunctions::print("LOADING TEXTURES");
// 		int texture_index = nexus->patches->texture;

// 		// Lade die Textur (du brauchst loadImageFromData ohne Qt!)
// 		nx::TextureData& tex_data = nexus->texturedata[texture_index];
// 		// nexus->loadImageFromData(tex_data, texture_index);

// 		UtilityFunctions::print("Texture index: ", texture_index);
// 		UtilityFunctions::print("Texture width: ", tex_data.width);
// 		UtilityFunctions::print("Texture height: ", tex_data.height);
// 		UtilityFunctions::print("Memory pointer: ", reinterpret_cast<uint64_t>(tex_data.memory));

// 		for (int i = 0; i < 10; ++i) {
// 			UtilityFunctions::print("Byte ", i, ": ", (uint8_t)tex_data.memory[i]);
// 		}

// 		// Erzeuge PackedByteArray aus raw RGBA-Daten
// 		int64_t img_size = tex_data.width * tex_data.height * 4;
// 		PackedByteArray byte_array;
// 		byte_array.resize(img_size); // width * height * 4

// 		{
// 			uint8_t* write_ptr = byte_array.ptrw();
// 			memcpy(write_ptr, tex_data.memory, img_size);
// 		}


// 		// Erzeuge Image aus dem raw RGBA-Buffer
// 		Ref<Image> image;
// 		image.instantiate();
// 		// image->create_from_data(
// 		// 	tex_data.width,
// 		// 	tex_data.height,
// 		// 	false, // no mipmaps
// 		// 	Image::FORMAT_RGBA8,
// 		// 	byte_array
// 		// );

// 		// // Versuche es mit dem Godot-JPEG-Decoder
// 		// Error err = image->load_jpg_from_buffer(byte_array);
// 		// if (err != OK) {
// 		// 	UtilityFunctions::print("Failed to decode JPEG buffer.");
// 		// 	return {};
// 		// }

// 		UtilityFunctions::print("PackedByteArray size: ", byte_array.size());
// 		UtilityFunctions::print("Image width: ", image->get_width());
// 		UtilityFunctions::print("Image height: ", image->get_height());
// 		UtilityFunctions::print("Image empty: ", image->is_empty());


// 		// Erzeuge ein Textur-Objekt
// 		Ref<ImageTexture> texture = ImageTexture::create_from_image(image);

// 		// Erzeuge Material
// 		Ref<StandardMaterial3D> material;
// 		material.instantiate();
// 		material->set_texture(BaseMaterial3D::TEXTURE_ALBEDO, texture);

// 		// Weise Material der ersten Surface zu
// 		mesh->surface_set_material(0, material);
// 	}


// 	return mesh;
// }

Ref<ArrayMesh> NexusNode::loadNexusModell(String url) {
	url = url.replace("res:/", ".");

	nx::Nexus* nexus = new nx::Nexus();
	nexus->filename = std::string(url.utf8().get_data());
	nexus->file->setFileName(url.utf8().get_data());
	nexus->file->open(nx::NexusFile::Read);
	nexus->loadHeader();
	nexus->loadIndex();
	nexus->loadRam(0);

	Ref<ArrayMesh> mesh;
	mesh.instantiate();

	std::map<uint32_t, Ref<ImageTexture>> texture_cache;

	// Schritt 1: Alle Patches von Node 0 sammeln
	nx::Node& node = nexus->nodes[0];
	nx::NodeData& data = nexus->nodedata[0];
	nx::Signature& sig = nexus->header.signature;

	uint32_t nvert = node.nvert;
	uint16_t* indices = data.faces(sig, nvert);
	vcg::Point3f* coords = data.coords();
	vcg::Point2f* uvs = data.texCoords(sig, nvert);
	vcg::Point3s* normals = data.normals(sig, nvert);

	UtilityFunctions::print("node.first_patch: ", node.first_patch);
	UtilityFunctions::print("node.last_patch(): ", node.last_patch());

	UtilityFunctions::print("node.nvert", node.nvert);
	UtilityFunctions::print("node.nface", node.nface);

	for (uint32_t k = node.first_patch; k < node.last_patch(); k++) {
		nx::Patch& patch = nexus->patches[k];
		uint32_t tex_index = patch.texture;
		uint32_t offset = 0;
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

		UtilityFunctions::print("tex_index: ", tex_index);
		UtilityFunctions::print("tex_index != 0xffffffff: ", (tex_index != 0xffffffff));
		UtilityFunctions::print("tex_index < nexus->header.n_textures: ", (tex_index < nexus->header.n_textures));

		// Schritt 2: Textur laden und Material zuweisen
		if (tex_index != 0xffffffff && tex_index < nexus->header.n_textures) {
			Ref<ImageTexture> texture;
			
			// Cache prüfen
			if (texture_cache.find(tex_index) != texture_cache.end()) {
				texture = texture_cache[tex_index];
			} else {
				// nx::TextureData& tex_data = nexus->texturedata[tex_index];
				// if (!tex_data.memory) {
				// 	UtilityFunctions::printerr("Tex index ", tex_index, " hat kein gültiges Speicherabbild (memory=nullptr).");
				// 	continue;
				// }

				// int tex_size = tex_data.width * tex_data.height * 4;
				// PackedByteArray byte_array;
				// byte_array.resize(tex_size);
				// memcpy(byte_array.ptrw(), tex_data.memory, tex_size);

				// UtilityFunctions::print("Byte array size: ", byte_array.size());

				// String temp_path = "user://temp_texture_" + String::num(tex_index) + ".jpg";
				// FileAccess* file = FileAccess::open(temp_path, FileAccess::WRITE);
				// file->store_buffer(byte_array);
				// file->close();

				// Ref<Image> image;
				// image.instantiate();
				// Error err = image->load(temp_path);
				// if (err != OK) {
				// 	UtilityFunctions::printerr("Fehler beim Laden von Textur ", tex_index, ": ", err);
				// 	continue;
				// }


				// Ref<Image> image;
				// image.instantiate();
				// image->create_from_data(tex_data.width, tex_data.height, false, Image::FORMAT_RGBA8, byte_array);

				// texture.instantiate();
				// texture->create_from_image(image);
				// texture_cache[tex_index] = texture;

				// Schritt 1: JPEG-Daten aus Nexus extrahieren
				nx::TextureData& tex_data = nexus->texturedata[tex_index];
				// int tex_size = tex_data.width * tex_data.height * 4; 
				int tex_size = nexus->textures[tex_index].getSize();
				UtilityFunctions::print("tex_size: ", tex_size);

				PackedByteArray byte_array;
				byte_array.resize(tex_size); // Verwende tatsächliche JPEG-Größe
				memcpy(byte_array.ptrw(), tex_data.memory, tex_size);

				// Schritt 2: Temporäre Datei schreiben
				String temp_path = "user://nexus_texture_" + itos(tex_index) + ".jpg";
				Ref<FileAccess> file = FileAccess::open(temp_path, FileAccess::WRITE);
				file->store_buffer(byte_array);
				file->close();

				// Schritt 3: Image laden
				Ref<Image> image;
				image.instantiate();
				Error err = image->load(temp_path);
				if (err != OK || image->is_empty()) {
					UtilityFunctions::printerr("Fehler beim Laden von Textur ", tex_index);
					continue;
				}

				// Schritt 4: ImageTexture erstellen
				// Ref<ImageTexture> texture;
				texture.instantiate();
				texture->create_from_image(image);
				texture_cache[tex_index] = texture;


			}

			// Material erstellen
			Ref<StandardMaterial3D> material;
			material.instantiate();
			material->set_texture(StandardMaterial3D::TEXTURE_ALBEDO, texture);
			mesh->surface_set_material(surface_index, material);
		}
	}

	return mesh;
}

