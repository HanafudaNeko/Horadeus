extends Ability

func _on_use(_caster):
	print("BAR!")
	return AbilityError.OK
