#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/thread.hpp>
#include "nexus.h"
#include <thread>

namespace godot {

class NexusNode : public MeshInstance3D {
	GDCLASS(NexusNode, MeshInstance3D)

private:

	std::thread::id main_thread_id;

	nx::Nexus* nexus;
    Ref<Thread> worker_thread;
    int current_node = 0;

protected:
	static void _bind_methods();

public:
	NexusNode();
	~NexusNode();

	void _on_node_loaded(Ref<ArrayMesh> mesh);
    bool start_loading(String url);
	Ref<ArrayMesh> loadNexusNode(int node_index);

	void _process(double delta) override;
};

}