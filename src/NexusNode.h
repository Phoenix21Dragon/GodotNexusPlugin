#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/array_mesh.hpp>

namespace godot {

class NexusNode : public Node3D {
	GDCLASS(NexusNode, Node3D)

private:

protected:
	static void _bind_methods();

public:
	NexusNode();
	~NexusNode();

    Ref<ArrayMesh> loadNexusModell(String url);

	void _process(double delta) override;
};

}