class_name Effect
extends Spatial

export(StreamTexture) var thumbnail = null
var pawn : Pawn


func _ready():
	set_physics_process(false)


func get_class() -> String:
	return "Effect"


func set_affected_pawn(affected_pawn):
	pawn = affected_pawn
	_setup()


func _exit_tree():
	_cleanup()


# =================================================
# ========= ONLY OVERLOAD FUNCTIONS BELOW =========
# =================================================

# Called every frame; This is where your effect code belongs
func tick(_delta):
	pass


# Returns the remaining duration on the effect; INF means it has no duration and is an aura
# As soon as duration reaches 0, the affected Pawn will remove it
func remaining_duration() -> float:
	return INF


# Anything you need to do to setup an effect (add a speed increase, apply particles, etc) belongs here
func _setup():
	pass


# Anything you need to do to cleanup an effect (remove a speed increase, remove particles, etc) belongs here
func _cleanup():
	pass
