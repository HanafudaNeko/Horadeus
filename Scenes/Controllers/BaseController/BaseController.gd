class_name Controller
extends Spatial


var is_active_controller := false setget _set_is_active_controller
var pawn setget _set_pawn


func _ready():
	set_physics_process(is_active_controller and pawn != null)


func get_class() -> String:
	return "Controller"


func _set_is_active_controller(is_active : bool):
	is_active_controller = is_active
	set_physics_process(is_active_controller and pawn != null)


func _set_pawn(new_pawn):
	pawn = new_pawn
	set_physics_process(is_active_controller and pawn != null)
	_after_set_pawn()


# ===============================================
# ====== ONLY OVERLOAD THE FUNCTIONS BELOW ======
# ===============================================

# All code that controls the pawn belongs in here
func _physics_process(_delta):
	pass


# Called after the pawn is set
# Any setup the controller needs to do should be in here
#	eg: Setting up action bars, setting up inventory UI, setting up Friendly/Hostile sensors
func _after_set_pawn():
	return
