extends Spatial

enum BillboardMode {Y_Billboard_Ease, Y_Billboard, Snap, Snap_Ease, Negated_Y_Billboard}
export(int, "Y Billboard Ease", "Y Billboard", "Snap", "Snap Ease", "Negated Y Billboard") var billboard_mode := BillboardMode.Y_Billboard_Ease

onready var entity_root = get_parent()
onready var animation_player = entity_root.get_node("AnimationPlayer")


# Whether or not the animation player h
var _has_two_front_animations := false

# Used when trying to decide between frontleft and frontright when the mode is Snap or Y Billboard Ease
var _true_degrees := 0.0


func _ready():
	if get_viewport().get_camera() ==  null: print("No 3D camera found in SpriteEntity.gd::_ready(); The class will not behave properly without a camera.")
	# Determine if the animation has two front animations (frontleft and frontright)
	for animation in animation_player.get_animation_list():
		if animation.find("frontleft") != -1: 
			_has_two_front_animations = true
			break


func _process(_delta):
	# Done in process because it is a visual effect; no need to clog up the physics engine
	var degrees = _get_billboard_degrees()
	rotation_degrees = Vector3(0.0, degrees, 0.0)
	_set_animation_direction(side_facing_camera())


# Sets the current animation for the Sprite3D
func set_animation(animation_name : String):
	var new_animation = side_facing_camera() + "_" + animation_name
	if animation_player.current_animation != new_animation:
		animation_player.current_animation = new_animation


# Returns which side of the SpriteEntity is currently facing the active camera
func side_facing_camera() -> String:
	# TODO: can we simplify this?
	if rotation_degrees.y <= 45.0 and rotation_degrees.y >= -45.0:
		if _has_two_front_animations:
			return "frontright" if _true_degrees > 0.0 else "frontleft"
		else:
			return "front"
	elif rotation_degrees.y < 135.0 and rotation_degrees.y >= 45.0:
		return "right"
	elif rotation_degrees.y > -135.0 and rotation_degrees.y <= -45.0:
		return "left"
	else:
		return "back"


# Returns the entitys current facing vector
func get_facing_vector() -> Vector3:
	return Vector3.FORWARD.rotated(Vector3.UP, deg2rad(entity_root.rotation_degrees.y))


# Sets the direction of the current animation
func _set_animation_direction(facing : String):
	var current_animation = animation_player.current_animation
	var new_animation = facing + current_animation.substr(current_animation.find("_"))
	
	if current_animation != new_animation and current_animation != "":
		var current_time = animation_player.current_animation_position
		animation_player.current_animation = new_animation
		animation_player.advance(current_time) # Advance the new animation to the frame of the last animation


# Returns the degree (based on billboard mode) that the Sprite3D should be facing the camera
func _get_billboard_degrees() -> float:
	var current_camera = get_viewport().get_camera()
	if current_camera ==  null:
		return 0.0
	
	# Get the camera position in local coordinates of the entity
	var camera_to_entity_local = entity_root.global_transform.basis.xform_inv(current_camera.global_transform.origin - entity_root.translation)
	_true_degrees = rad2deg(Vector2(camera_to_entity_local.x, camera_to_entity_local.z).angle_to(Vector2(0.0, -1.0)))
	
	match billboard_mode:
		BillboardMode.Y_Billboard_Ease:
			var start_ease_at = 30.0
			var clamped_angle_90 = 90.0*floor((_true_degrees+45.0)/90.0)
			var degrees_from_clamped_value = clamped_angle_90 - _true_degrees
			var ease_strength = (degrees_from_clamped_value/(45.0-start_ease_at)) - (start_ease_at/(45.0-start_ease_at))*(1.0 if degrees_from_clamped_value >= 0.0 else -1.0)
			return clamped_angle_90 if abs(degrees_from_clamped_value) <= start_ease_at else clamped_angle_90 - (45.0*ease_strength)
		BillboardMode.Y_Billboard:
			return _true_degrees
		BillboardMode.Snap:
			return 90.0*floor((_true_degrees+45.0)/90.0)
		BillboardMode.Snap_Ease:
			return (_true_degrees+(90.0*floor((_true_degrees+45.0)/90.0)))/2.0
		BillboardMode.Negated_Y_Billboard:
			return -_true_degrees
		var invalid_mode:
			print("Invalid billboard_mode in " + entity_root.name + ": " + str(invalid_mode))
			return _true_degrees
