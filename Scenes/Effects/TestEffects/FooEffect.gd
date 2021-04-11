extends Effect

var duration = 6.0
var speed_boost = 0.5

# Called every frame; This is where your effect code belongs
func tick(delta):
	duration -= delta


# Returns the remaining duration on the effect; INF means it has no duration and is an aura
# As soon as duration reaches 0, the affected Pawn will remove it
func remaining_duration() -> float:
	return duration


# Anything you need to do to setup an effect (add a speed increase, apply particles, etc) belongs here
func _setup():
	affected_pawn.speed += speed_boost
	affected_pawn.get_node("AnimationPlayer").playback_speed += speed_boost


# Anything you need to do to cleanup an effect (remove a speed increase, remove particles, etc) belongs here
func _cleanup():
	affected_pawn.speed -= speed_boost
	affected_pawn.get_node("AnimationPlayer").playback_speed -= speed_boost
