#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class NexusNode : public Node3D {
	GDCLASS(NexusNode, Node3D)

private:

protected:
	static void _bind_methods();

public:
	NexusNode();
	~NexusNode();

    void loadNexusModell(String url);

	void _process(double delta) override;
};

}