#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include "nexus.h"
#include "renderer.h"
#include "scene.h"

namespace godot {

class NexusNode : public Node3D {
	GDCLASS(NexusNode, Node3D)

private:

	nx::Nexus* nexus;
	nx::Renderer renderer;
	Scene scene;

protected:
	static void _bind_methods();
	bool extracting;

public:
	NexusNode();
	~NexusNode();
	void init(String url, Dictionary options);
    bool openNexusModell(String url);
	Ref<ArrayMesh> loadNexusNode(int node_index);

	void _process(double delta) override;
};

}