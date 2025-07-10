#include "NexusNode.h"
#include <godot_cpp/core/class_db.hpp>
#include "nexus.h"

#include <stdio.h>
#include <string>

#include "nexus/src/common/signature.h"
#include "nexus/src/common/dag.h"

#include <vcg/space/color4.h>
#include "nexus/src/common/nexusfile.h"

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

void NexusNode::loadNexusModell(String url){
    UtilityFunctions::print("LOAD NEXUS MODELL url=", url);

	// nx::Nexus* nexus = new nx::Nexus(NULL);

	UtilityFunctions::print("Created Nexus");
	
	// godot::CharString url_char_str = url.utf8();
	// const char* url_char = url_char_str.get_data();

	// auto result = nexus->open(url_char); 
}