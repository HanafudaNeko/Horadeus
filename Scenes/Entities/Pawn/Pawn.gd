class_name Pawn
extends KinematicBody

export(int, "Player", "Town", "Critter", "Monster") var allegiance_layer = 1
export(int, FLAGS, "Player", "Town", "Critter", "Monster") var allegiance_mask = 3

const GRAVITY = Vector3.DOWN
var gravity_mod := 1.0

var forward_vector := Vector3.ZERO setget set_forward_vector
var speed := 1.0

# Signals
signal ability_gained
signal item_gained
signal effect_gained


func _ready():
	_check_for_stray_nodes()
	_set_active_controller()


func _physics_process(delta):
	for effect in get_effects():
		effect.tick(delta)
		if effect.remaining_duration() <= 0.0:
			effect.queue_free()
	move_and_slide((forward_vector * speed) + (GRAVITY*gravity_mod), Vector3.UP)


func get_class() -> String:
	return "Pawn"


# Returns whether the passed entity is friendly
func is_friendly(other) -> bool:
	return other.allegiance_mask & (1 << allegiance_layer) != 0


func set_forward_vector(new_forward : Vector3, look_down_forward := true):
	forward_vector = new_forward
	# Point the Pawn in the direction of the new forward_vector if it is not zero
	new_forward = (new_forward*Vector3(1.0,0.0,1.0)).normalized()
	if look_down_forward and new_forward.length_squared() > 0.0:
		rotation_degrees.y = (-1.0 if new_forward.x >= 0.0 else 1.0) * rad2deg(Vector3.FORWARD.angle_to(new_forward))
		$SpriteRoot.set_animation("walk") # TODO: move all animation logic to an AnimationTree
	else:
		$SpriteRoot.set_animation("idle") # TODO: move all animation logic to an AnimationTree


func add_ability(ability):
	$Abilities.add_child(ability)
	emit_signal("ability_gained", ability)


func add_item(item):
	$Inventory.add_child(item)
	emit_signal("item_gained", item)


func add_effect(effect):
	$Effects.add_child(effect)
	effect.set_affected_pawn(self)
	emit_signal("effect_gained", effect)


func get_abilities():
	return $Abilities.get_children()


func get_inventory():
	return $Inventory.get_children()


func get_effects():
	return $Effects.get_children()


func _set_active_controller():
	var child_count = $Controllers.get_child_count()
	# If we have at least one controller...
	if child_count > 0:
		# Make first one the active controller
		var controller = $Controllers.get_child(0)
		controller.is_active_controller = true
		controller.pawn = self
		# And disable the rest
		for i in range(1,child_count):
			$Controllers.get_child(i).is_active_controller = false


# Checks to see if there are any stray controllers, abilities, items, or effects outside of their respective Spatial node
func _check_for_stray_nodes():
	for child in get_children():
		match child.get_class():
			"Controller":
				# TODO: will need to change this to iterate in reverse at some point so order of stray controllers is retained
				remove_child(child)
				$Controllers.add_child(child)
				$Controllers.move_child(child, 0)
			"Ability":
				remove_child(child)
				$Abilities.add_child(child)
			"Item":
				remove_child(child)
				$Inventory.add_child(child)
			"Effect":
				remove_child(child)
				$Effects.add_child(child)
