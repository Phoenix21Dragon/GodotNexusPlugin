@tool
extends EditorImportPlugin

enum Presets { DEFAULT }

var magic: int
var version: int
var nvert: int
var nface: int
var signature: PackedByteArray
var n_nodes: int
var n_patches: int
var n_textures: int
var sphere_center: Vector3
var sphere_radius: float

var nexus_node : NexusNode = NexusNode.new()

func _get_import_order():
	return 0

func _get_priority():
	return 1.0

func _get_importer_name():
	return "demos.nexusmodell"
	
func _get_visible_name():
	return "Nexus Modell"

func _get_recognized_extensions():
	return ["nxs", "nxz"]

func _get_save_extension():
	return "tscn"

func _get_resource_type():
	return "PackedScene"

func _get_preset_count():
	return Presets.size()

func _get_preset_name(preset_index):
	match preset_index:
		Presets.DEFAULT:
			return "Default"
		_:
			return "Unknown"

func _get_import_options(path, preset_index):
	match preset_index:
		Presets.DEFAULT:
			return [
				{
				   "name": "max_number_triangles",
				   "default_value": 3.0,
					"property_hint": PROPERTY_HINT_RANGE, 
				   "hint_string": "0.5,15.0,0.5"
				},
				{
				   "name": "target_error",
				   "default_value": 3.0,
					"property_hint": PROPERTY_HINT_RANGE, 
				   "hint_string": "1,50,1"
				},
				{
				   "name": "ram",
				   "default_value": 500.0,
					"property_hint": PROPERTY_HINT_RANGE, 
				   "hint_string": "100,10000,100"
				},
				{
				   "name": "gpu",
				   "default_value": 500.0,
					"property_hint": PROPERTY_HINT_RANGE, 
				   "hint_string": "100,10000,100"
				},
				{
				   "name": "fps",
				   "default_value": 30.0,
					"property_hint": PROPERTY_HINT_RANGE, 
				   "hint_string": "30.0,120.0,5"
				},
				{
				   "name": "instances",
				   "default_value": 1,
					"property_hint": PROPERTY_HINT_RANGE, 
				   "hint_string": "1,5,1"
				},
				{
				   "name": "autopositioning",
				   "default_value": false,
				}
			]
		_:
			return []

func _get_option_visibility(path, option_name, options):
	return true

func _import(source_file, save_path, options, r_platform_variants, r_gen_files):

	nexus_node.init(source_file, options)

	# ########## Read nxs File with Nexus C++ Code ##########	

	# var success = nexus_node.openNexusModell(source_file)
	# print("success open: ", success)

	# # Neue Szene erzeugen mit Node3D als Root
	# var root := nexus_node
	# root.name = "ImportedNexusModel"

	# for node in range(0, 2):
		
	# 	var mesh = nexus_node.loadNexusNode(node)

	# 	if mesh == null:
	# 		push_error("MeshInstance konnte nicht geladen werden.")
	# 		return ERR_CANT_CREATE
		
	# 	var mesh_instance = MeshInstance3D.new()
	# 	mesh_instance.name = "Node_%d" % node
	# 	mesh_instance.mesh = mesh
	# 	root.add_child(mesh_instance)
	# 	mesh_instance.owner = root  # Wichtig für PackedScene

	# # Szene packen und speichern
	# var scene := PackedScene.new()
	# scene.pack(root)

	# var save_result := ResourceSaver.save(scene, "%s.%s" % [save_path, _get_save_extension()])
	# if save_result != OK:
	# 	push_error("Konnte Szene nicht speichern: %s" % save_result)
	# return save_result

func read_header(file: FileAccess) -> void:
	print("Read Header")
	
	magic = file.get_32()
	if magic != 0x4E787320:
		push_error("Invalid magic number, not a Nexus file")
		return
	
	version = file.get_32()
	print("    ", "version: ", version)
	nvert = file.get_64()
	print("    ", "nvert: ", nvert)
	nface = file.get_64()
	print("    ", "nface: ", nface)

	var vertex_attributes = []
	var face_attributes = []
	var flags = 0

	# Vertex attributes
	for i in range(8):
		var type = file.get_8()
		var number = file.get_8()
		vertex_attributes.append({ "type": type, "number": number })
	#print("    ", vertex_attributes)

	# Face attributes
	for i in range(8):
		var type = file.get_8()
		var number = file.get_8()
		face_attributes.append({ "type": type, "number": number })
	#print("    ", face_attributes)

	flags = file.get_32()	
	print("    ", "flags: ", flags)

	n_nodes = file.get_32()
	print("    ", "n_nodes: ", n_nodes)
	n_patches = file.get_32()
	print("    ", "n_patches: ", n_patches)
	n_textures = file.get_32()
	print("    ", "n_textures: ", n_textures)

	var x = file.get_float()
	var y = file.get_float()
	var z = file.get_float()
	var r = file.get_float()
	sphere_center = Vector3(x, y, z)
	sphere_radius = r
	print("    ", "sphere_center: ", sphere_center)
	print("    ", "sphere_radius: ", sphere_radius)

	print("Header loaded successfully.")
