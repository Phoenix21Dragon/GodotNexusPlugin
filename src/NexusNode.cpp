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
#include "vcg/space/point3.h"
#include "vcg/space/point2.h"

#include "nexus.h"

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

Ref<ArrayMesh> NexusNode::loadNexusModell(String url){

	url = url.replace("res:/", ".");
	
	nx::Nexus* nexus = new nx::Nexus();
	nexus->filename = std::string(url.utf8().get_data());
	nexus->file->setFileName(url.utf8().get_data());
	nexus->file->open(nx::NexusFile::Read);
	nexus->loadHeader();
	UtilityFunctions::print("magic: ", nexus->header.magic);
	UtilityFunctions::print("version: ", nexus->header.version);
	UtilityFunctions::print("nvert: ", nexus->header.nvert);
	UtilityFunctions::print("nface: ", nexus->header.nface);
	UtilityFunctions::print("n_nodes: ", nexus->header.n_nodes);
	UtilityFunctions::print("n_patches: ", nexus->header.n_patches);
	UtilityFunctions::print("n_textures: ", nexus->header.n_textures);
	
	nexus->loadIndex();
	// UtilityFunctions::print("nexus->nodes[0].offset: ", nexus->nodes[0].offset);
	// UtilityFunctions::print("nexus->nodes[0].getBeginOffset: ", nexus->nodes[0].getBeginOffset());
	// UtilityFunctions::print("nexus->nodes[0].getEndOffset: ", nexus->nodes[0].getEndOffset());
	// UtilityFunctions::print("nexus->nodes[0].getSize: ", nexus->nodes[0].getSize());

	// UtilityFunctions::print("nexus->nodes[1].offset: ", nexus->nodes[1].offset);
	// UtilityFunctions::print("nexus->nodes[1].getBeginOffset: ", nexus->nodes[1].getBeginOffset());
	// UtilityFunctions::print("nexus->nodes[1].getEndOffset: ", nexus->nodes[1].getEndOffset());
	// UtilityFunctions::print("nexus->nodes[1].getSize: ", nexus->nodes[1].getSize());
	
	// UtilityFunctions::print("nexus->nodes[2].offset: ", nexus->nodes[2].offset);
	// UtilityFunctions::print("nexus->nodes[2].getBeginOffset: ", nexus->nodes[2].getBeginOffset());
	// UtilityFunctions::print("nexus->nodes[2].getEndOffset: ", nexus->nodes[2].getEndOffset());
	// UtilityFunctions::print("nexus->nodes[2].getSize: ", nexus->nodes[2].getSize());
	
	// UtilityFunctions::print("nexus->patches->texture: ", nexus->patches->texture);
	// UtilityFunctions::print("nexus->textures->offset: ", nexus->textures->offset);

	nexus->loadRam(0);

	nx::NodeData& node_data = nexus->nodedata[0];
	nx::Node& node = nexus->nodes[0];

	uint32_t nvert = node.nvert;
	uint32_t nface = node.nface;

	vcg::Point3f* coords = node_data.coords();
	vcg::Point2f* uvs = node_data.texCoords(nexus->header.signature, nvert);
	vcg::Point3s* normals = node_data.normals(nexus->header.signature, nvert);
	uint16_t* indices = node_data.faces(nexus->header.signature, nvert);

	PackedVector3Array godot_vertices;
	PackedVector3Array godot_normals;
	PackedVector2Array godot_uvs;
	PackedInt32Array godot_indices;

	UtilityFunctions::print("coords[0].X(): ", coords[0].X());
	UtilityFunctions::print("coords[1].X(): ", coords[1].X());
	UtilityFunctions::print("coords[2].X(): ", coords[2].X());

	for (uint32_t i = 0; i < nvert; ++i) {
		godot_vertices.push_back(Vector3(coords[i].X(), coords[i].Y(), coords[i].Z()));
		if (normals)
			godot_normals.push_back(Vector3(normals[i].X(), normals[i].Y(), normals[i].Z()));
		if (uvs)
			godot_uvs.push_back(Vector2(uvs[i].X(), uvs[i].Y()));
	}

	for (uint32_t i = 0; i < nface * 3; ++i) {
		godot_indices.push_back(indices[i]);
	}

	Array arrays;
	arrays.resize(Mesh::ARRAY_MAX);
	arrays[Mesh::ARRAY_VERTEX] = godot_vertices;
	arrays[Mesh::ARRAY_NORMAL] = godot_normals;
	arrays[Mesh::ARRAY_TEX_UV] = godot_uvs;
	arrays[Mesh::ARRAY_INDEX] = godot_indices;

	Ref<ArrayMesh> mesh;
	mesh.instantiate();
	UtilityFunctions::print("IS VALID: ", mesh.is_valid());
 	mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);

	return mesh;
}