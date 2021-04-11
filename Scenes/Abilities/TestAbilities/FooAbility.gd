extends Ability

func _on_use(caster):
	var effect = load("res://Scenes/Effects/TestEffects/FooEffect.tscn").instance()
	caster.add_effect(effect)
	return AbilityError.OK
