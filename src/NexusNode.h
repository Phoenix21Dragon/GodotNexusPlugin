#include <godot_cpp/classes/mesh_instance3d.hpp>
// #include <godot_cpp/classes/array_mesh.hpp>
#include "nexus.h"

namespace godot {

class NexusNode : public MeshInstance3D {
	GDCLASS(NexusNode, MeshInstance3D)

private:

	nx::Nexus* nexus;
	String url;

protected:
	static void _bind_methods();
	bool extracting;

public:
	NexusNode();
	~NexusNode();
	void init(String url, Dictionary options);
    bool openNexusModell(String url);
	Ref<ArrayMesh> loadNexusNode(int node_index);

	void _ready() override;
	void _process(double delta) override;

	String get_url() const;
	void set_url(const String p_url);

};

}