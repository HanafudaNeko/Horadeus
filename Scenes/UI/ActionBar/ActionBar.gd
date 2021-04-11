extends Control

var linked_abilities = {}

signal action_button_pressed


func _ready():
	$GridContainer/ActionButton1.connect("action_button_pressed", self, "_on_action_button_pressed")
	$GridContainer/ActionButton2.connect("action_button_pressed", self, "_on_action_button_pressed")
	$GridContainer/ActionButton3.connect("action_button_pressed", self, "_on_action_button_pressed")
	$GridContainer/ActionButton4.connect("action_button_pressed", self, "_on_action_button_pressed")
	$GridContainer/ActionButton5.connect("action_button_pressed", self, "_on_action_button_pressed")
	$GridContainer/ActionButton6.connect("action_button_pressed", self, "_on_action_button_pressed")


func _process(_delta):
	# TODO: Update actionbar gui (cooldowns and ability cost)
	pass


func link_ability(ability : Ability, action_num : int):
	if ability != null:
		linked_abilities[action_num] = ability
		$GridContainer.get_node("ActionButton" + str(action_num)).set_icon(ability.thumbnail)
	elif linked_abilities.has(action_num):
		linked_abilities.erase(action_num)
		$GridContainer.get_node("ActionButton" + str(action_num)).set_icon(null)


func _input(event):
	for action_num in range(1,7):
		if !linked_abilities.has(action_num):
			continue
		if event.is_action_pressed("action_" + str(action_num)):
			$GridContainer.get_node("ActionButton" + str(action_num)).press_modulate()
		if event.is_action_released("action_" + str(action_num)):
			$GridContainer.get_node("ActionButton" + str(action_num)).normal_modulate()
			emit_signal("action_button_pressed", linked_abilities[action_num])


func _on_action_button_pressed(action_num : int):
	if linked_abilities.has(action_num):
		emit_signal("action_button_pressed", linked_abilities[action_num])
