class_name Ability
extends Spatial

enum AbilityError {OK, ON_COOLDOWN, REQUIREMENTS_NOT_MET}

export(StreamTexture) var thumbnail = null
export(float) var cooldown := 1.0

var useable_in = 0.0


func _physics_process(delta):
	if useable_in > 0.0:
		useable_in = clamp(useable_in - delta, 0.0, INF)


func get_class() -> String:
	return "Ability"


# Returns whether the ability is off cooldown
func is_ready() -> bool:
	return useable_in <= 0.0


func use(caster) -> int:
	if is_ready():
		useable_in = cooldown
		return _on_use(caster)
	else:
		return AbilityError.ON_COOLDOWN


# =================================================
# ========= ONLY OVERLOAD FUNCTIONS BELOW =========
# =================================================

# Add Ability functionality here
func _on_use(caster) -> int:
	push_error(caster.name + " casted " + name + ", which has not been implemented")
	return AbilityError.OK


# Returns cost to cast ability; Empty array means no cost
# Needs to be in format [[{item_1_id}, {item_1_quantity}], [{item_2_id}, {item_2_quantity}], ...]
func cost() -> Array:
	return []


# Returns how many times the ability can be cast with the passed resources; INF means unlimited
func have_resources_for(caster) -> int:
	return int(INF)
