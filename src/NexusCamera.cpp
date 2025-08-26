#include "NexusCamera.h"
// #include "NexusNode.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include "scene.h"
#include "controller.h"
#include "renderer.h"
#include "nexus.h"

using namespace godot;

void NexusCamera::_bind_methods() {
	// ClassDB::bind_method(D_METHOD("get_nexus_nodes"), &NexusCamera::get_nexus_nodes);
	// ClassDB::bind_method(D_METHOD("set_nexus_nodes", "nodes"), &NexusCamera::set_nexus_nodes);
	// ADD_PROPERTY(PropertyInfo(Variant::ARRAY, 
	// 						  "nexus_nodes", 
	// 						  PROPERTY_HINT_TYPE_STRING, 
	// 						  String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + "NexusNode"),
	// 						//   "NexusNode:"),
	// 			 "set_nexus_nodes", 
	// 			 "get_nexus_nodes");

	ClassDB::bind_method(D_METHOD("get_extracting"), &NexusCamera::get_extracting);
	ClassDB::bind_method(D_METHOD("set_extracting", "p_extracting"), &NexusCamera::set_extracting);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "extracting"), "set_extracting", "get_extracting");

	ClassDB::bind_method(D_METHOD("get_ram"), &NexusCamera::get_ram);
	ClassDB::bind_method(D_METHOD("set_ram", "p_ram"), &NexusCamera::set_ram);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "ram"), "set_ram", "get_ram");

	ClassDB::bind_method(D_METHOD("get_gpu"), &NexusCamera::get_gpu);
	ClassDB::bind_method(D_METHOD("set_gpu", "p_gpu"), &NexusCamera::set_gpu);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "gpu"), "set_gpu", "get_gpu");

	ClassDB::bind_method(D_METHOD("get_max_number_triangles"), &NexusCamera::get_max_number_triangles);
	ClassDB::bind_method(D_METHOD("set_max_number_triangles", "p_max_number_triangles"), &NexusCamera::set_max_number_triangles);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_number_triangles"), "set_max_number_triangles", "get_max_number_triangles");

	ClassDB::bind_method(D_METHOD("get_target_error"), &NexusCamera::get_target_error);
	ClassDB::bind_method(D_METHOD("set_target_error", "p_target_error"), &NexusCamera::set_target_error);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "target_error"), "set_target_error", "get_target_error");

	ClassDB::bind_method(D_METHOD("get_fps"), &NexusCamera::get_fps);
	ClassDB::bind_method(D_METHOD("set_fps", "p_fps"), &NexusCamera::set_fps);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "fps"), "set_fps", "get_fps");
}

NexusCamera::NexusCamera(): extracting(true), 
							ram(500.0f), 
							gpu(500.0f), 
							max_number_triangles(3), 
							target_error(3.0f),
							fps(30.0f) {
	// Initialize any variables here.
}

NexusCamera::~NexusCamera() {
	// Add your cleanup here.
}

void NexusCamera::_ready() {
    QStringList urls;

    // for (int i = 0; i < get_child_count(); i++) {
    //     Node *child = get_child(i);
    //     if (auto *nexus_node = Object::cast_to<NexusNode>(child)) {
    //         String godot_url = nexus_node->get_url(); 
	// 		godot_url = godot_url.replace("res:/", ".");
    //         urls << QString::fromUtf8(godot_url.utf8().get_data());
    //     }
    // }

	Node* parent = get_parent();
    if (!parent) return;

    int child_count = parent->get_child_count();
    for (int i = 0; i < child_count; ++i) {
        Node* child = parent->get_child(i);

		// Prüfen ob es die NexusCamera nicht selbst ist
        if (child == this) continue;

        // Prüfen ob es ein NexusNode ist
        if (Object::cast_to<NexusNode>(child)) {
            NexusNode* nexus_node = Object::cast_to<NexusNode>(child);
			nexus_nodes.append(nexus_node);

			// Konvertieren der url von res://path.nxs zu ./path.nxs
			String godot_url = nexus_node->get_url(); 
			godot_url = godot_url.replace("res:/", ".");
            urls << QString::fromUtf8(godot_url.utf8().get_data());
		}
    }

	scene.autopositioning = autopositioning;
	scene.load(urls, 1);

	nx::Controller &controller = scene.controller;
	controller.setRam((ram*(1<<20)));	
	controller.setGpu((gpu*(1<<20)));	
	controller.start();

	renderer.setMaxPrimitives(max_number_triangles*(1<<20));
	renderer.setError(target_error);
	renderer.setFps(fps);
}

void NexusCamera::_process(double delta) {
	
	/* only selected code from GLNxsview::paintEvent() */
	
	// scene.update();

	// if (renderer.frame > 4) return;

	renderer.startFrame();

	for(unsigned int i = 0; i < nexus_nodes.size(); i++) {
		Scene::Node &node = scene.nodes[i];
		renderer.update_camera_data(this);
		Ref<ArrayMesh> mesh = renderer.render(node.nexus, extracting);

		if (!mesh.is_valid() || mesh.is_null()) continue;
		
		if (NexusNode* nexus_node = Object::cast_to<NexusNode>(nexus_nodes[i])) // korrekt casten
        {
			// MeshInstance3D *meshInstance = memnew(MeshInstance3D);
			// meshInstance->set_mesh(mesh);
			// nexus_node->add_child(meshInstance);
	
			nexus_node->set_mesh(mesh);
        }
}

	if(extracting)
		renderer.endFrame();

	UtilityFunctions::print("number of rendered nodes: ", renderer.stats.node_rendered);  
	UtilityFunctions::prints("rendered nodes: ");
	for (size_t i = 0; i < renderer.selected.size(); i++)
	{
		if ((renderer.selected[i] == true) && (renderer.skipNode(i) == false)) 
			UtilityFunctions::print("  ", i);
	}
}

   
TypedArray<NexusNode> NexusCamera::get_nexus_nodes() const { 
	return nexus_nodes; 
} 
void NexusCamera::set_nexus_nodes(const TypedArray<NexusNode> &nodes) { 
	nexus_nodes = nodes; 
}   

// NexusNode NexusCamera::get_nexus_node() const { 
// 	return nexus_node; 
// } 
// void NexusCamera::set_nexus_node(const NexusNode &p_node) { 
// 	nexus_node = p_node; 
// }

bool NexusCamera::get_extracting() const {
	return extracting;
}
void NexusCamera::set_extracting(const bool p_extracting) {
	extracting = p_extracting;
}

bool NexusCamera::get_autopositioning() const {
	return autopositioning;
}
void NexusCamera::set_autopositioning(const bool p_autopositioning) {
	autopositioning = p_autopositioning;
}

int64_t NexusCamera::get_ram() const {
	return (int64_t)ram;
}
void NexusCamera::set_ram(const int64_t p_ram) {
	ram = (uint64_t)p_ram;
	scene.controller.setRam((ram*(1<<20)));
}

int64_t NexusCamera::get_gpu() const {
	return (int64_t)gpu;
}
void NexusCamera::set_gpu(const int64_t p_gpu) {
	gpu = (uint64_t)p_gpu;	
	scene.controller.setGpu((gpu*(1<<20)));
}

int64_t NexusCamera::get_max_number_triangles() const {
	return (int64_t)max_number_triangles;
}
void NexusCamera::set_max_number_triangles(const int64_t p_max_number_triangles) {
	max_number_triangles = (uint32_t)p_max_number_triangles;
	renderer.setMaxPrimitives(max_number_triangles*(1<<20));
}

float NexusCamera::get_target_error() const {
	return target_error;
}
void NexusCamera::set_target_error(const float p_target_error) {
	target_error = p_target_error;
	renderer.setError(target_error);
}

float NexusCamera::get_fps() const {
	return fps;
}
void NexusCamera::set_fps(const float p_fps) {
	fps = p_fps;
	renderer.setFps(fps);
}