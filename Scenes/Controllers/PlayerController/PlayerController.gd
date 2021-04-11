extends Controller

onready var camera_dolly = $CameraDolly
onready var camera = $CameraDolly/Camera

var _turn_speed := 2.5


func _ready():
	$UI/ActionBar.connect("action_button_pressed", self, "_on_ability_used")


func _on_ability_used(ability):
	# Only use ability if I am the active controller and have a pawn
	if is_active_controller and pawn != null:
		var error = ability.use(pawn)
		if error !=  OK:
			match error:
				Ability.AbilityError.ON_COOLDOWN:
					print("You can't cast " + ability.name + " for another " + str(ability.useable_in) + "s")
				Ability.AbilityError.REQUIREMENTS_NOT_MET:
					print("Insufficient resources")
			


func _after_set_pawn():
	_refresh_action_bar()


func _refresh_action_bar():
	var count = 1
	if pawn != null:
		for ability in pawn.get_abilities():
			$UI/ActionBar.link_ability(ability, count)
			count += 1
		# Set remaining buttons to null
		for i in range(count, 7):
			$UI/ActionBar.link_ability(null, i)


func _physics_process(delta):
	var local_forward = _compute_local_forward_vector()
	
	# Check X axis (Is player turning?)
	if local_forward.x != 0.0:
		pawn.rotate(Vector3.UP, -local_forward.x * delta * _turn_speed)
		local_forward = (local_forward * Vector3(0.0, 1.0, 1.0)).normalized() # Remove X from forward
		
	pawn.set_forward_vector(local_forward.rotated(Vector3.UP, deg2rad(pawn.rotation_degrees.y)), local_forward.z <= 0)


# Returns the forward vector of the player controler in it's local coordinates
func _compute_local_forward_vector() -> Vector3:
	var forward = Vector3.ZERO
	forward.z = Input.get_action_strength("back") - Input.get_action_strength("forward")
	forward.x = Input.get_action_strength("right") - Input.get_action_strength("left")
	return forward.normalized()
