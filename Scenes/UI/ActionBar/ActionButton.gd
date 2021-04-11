extends Control

export var action_number = 0
signal action_button_pressed

const HOVER_MODULATE := Color(1.2,1.2,1.2)
const PRESS_MODULATE := Color(0.7,0.7,0.7)


func _ready():
	$TextureButton/Label.text = str(action_number)


func set_icon(icon : Texture):
	$TextureButton.texture_normal = icon
	if icon == null:
		$TextureButton/Background.visible = true
	else:
		$TextureButton/Background.visible = false


func hover_modulate():
	$TextureButton/Background.modulate = HOVER_MODULATE
	$TextureButton.modulate = HOVER_MODULATE


func normal_modulate():
	$TextureButton/Background.modulate = Color.white
	$TextureButton.modulate = Color.white


func press_modulate():
	$TextureButton/Background.modulate = PRESS_MODULATE
	$TextureButton.modulate = PRESS_MODULATE


func _on_TextureButton_mouse_entered():
	hover_modulate()


func _on_TextureButton_mouse_exited():
	normal_modulate()


func _on_Background_mouse_entered():
	hover_modulate()


func _on_Background_mouse_exited():
	normal_modulate()


func _on_TextureButton_button_down():
	press_modulate()


func _on_TextureButton_button_up():
	normal_modulate()
	emit_signal("action_button_pressed", action_number)
