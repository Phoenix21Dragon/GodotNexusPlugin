#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include "nexus.h"

namespace godot {

class NexusNode : public Node3D {
	GDCLASS(NexusNode, Node3D)

private:

	nx::Nexus* nexus;

protected:
	static void _bind_methods();

public:
	NexusNode();
	~NexusNode();

    bool openNexusModell(String url);
	Ref<ArrayMesh> loadNexusNode(String url, int node_index);

	void _process(double delta) override;
};

}