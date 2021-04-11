extends Control

func _process(_delta):
	if Input.is_action_just_pressed("toggle_fps"):
		visible = !visible
	if visible:
		$FPSNumber.text = str(floor(Engine.get_frames_per_second()))
