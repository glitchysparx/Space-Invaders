#include "gameEffects.h"


void ExplosionEffect::OnConstruct()
{
	animation.AddFrame(AnimationFrame("assets/gfx/explosion/explosion2.png", Vector2D(50.f)));
	animation.AddFrame(AnimationFrame("assets/gfx/explosion/explosion3.png", Vector2D(50.f)));
	animation.AddFrame(AnimationFrame("assets/gfx/explosion/explosion5.png", Vector2D(50.f)));
	animation.AddFrame(AnimationFrame("assets/gfx/explosion/explosion7.png", Vector2D(50.f)));
	animation.AddFrame(AnimationFrame("assets/gfx/explosion/explosion8.png", Vector2D(50.f)));

	animation.SetUniformFrameDuration(4);
}