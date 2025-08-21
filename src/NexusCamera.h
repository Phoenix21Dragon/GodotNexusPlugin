// #include <godot_cpp/classes/node3d.hpp>
// #include <godot_cpp/classes/array_mesh.hpp>
// #include <godot_cpp/classes/camera3d.hpp>
// #include "nexus.h"

#include "NexusNode.h"
#include "renderer.h"
#include "scene.h"

namespace godot {

class NexusCamera : public  Camera3D{
	GDCLASS(NexusCamera, Camera3D)

private:

    TypedArray<NexusNode> nexus_nodes;
    // NexusNode* nexus_node;

	// nx::Nexus* nexus;
	nx::Renderer renderer;
	Scene scene;
    bool extracting;
    bool autopositioning;
    uint64_t ram;
    uint64_t gpu;
    uint32_t max_number_triangles;
    float target_error;
    float fps;
    
protected:
	static void _bind_methods();
    
public:
	NexusCamera();
	~NexusCamera();
    
	void _ready() override;
	void _process(double delta) override;

    TypedArray<NexusNode> get_nexus_nodes() const;
    void set_nexus_nodes(const TypedArray<NexusNode> &nodes);

    // NexusNode* get_nexus_node() const;
    // void set_nexus_node(const NexusNode *p_node);
    
    bool get_extracting() const;
    void set_extracting(const bool p_extracting);
    
    bool get_autopositioning() const;
    void set_autopositioning(const bool p_autopositioning);

    int64_t get_ram() const;
    void set_ram(const int64_t p_ram);

    int64_t get_gpu() const;
    void set_gpu(const int64_t p_gpu);

    int64_t get_max_number_triangles() const;
    void set_max_number_triangles(const int64_t p_max_number_triangles);

    float get_target_error() const;
    void set_target_error(const float p_target_error);

    float get_fps() const;
    void set_fps(const float p_fps);

    // bool autopositioning;
    // uint64_t ram;
    // uint64_t gpu_ram;
	
};

}