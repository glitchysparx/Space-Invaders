#include "gameEffects.h"


void ExplosionEffect::OnConstruct()
{
	animationFrames.reserve(5);

	animationFrames.push_back(AnimationFrame("gfx/explosion/explosion2.png", Vector2D(50.f)));
	animationFrames.push_back(AnimationFrame("gfx/explosion/explosion3.png", Vector2D(50.f)));
	animationFrames.push_back(AnimationFrame("gfx/explosion/explosion5.png", Vector2D(50.f)));
	animationFrames.push_back(AnimationFrame("gfx/explosion/explosion7.png", Vector2D(50.f)));
	animationFrames.push_back(AnimationFrame("gfx/explosion/explosion8.png", Vector2D(50.f)));

	uniformFrameDuration = 4;
}