#include "../../framework/graphics/Colors.h"
#include "Enemy.h"
#include "../effects/VFXSystem.h"

void Enemy::Construct(ResourceManager* resourceManager, VFXSystem* vfxSystem)
{
	if (!resourceManager || sprite) { return; }

	this->vfxSystem = vfxSystem;
	this->resourceManager = resourceManager;

	animation.AddFrame(AnimationFrame("assets/gfx/characters/LittleInvader_0.png", size));
	animation.AddFrame(AnimationFrame("assets/gfx/characters/LittleInvader_2.png", size));

	animation.SetLooping(true);

	sprite = resourceManager->GetSprite(animation.GetCurrentFrame().imagePath);
	soundExplosion = resourceManager->GetSound("assets/sounds/sfx/collision.wav", false);

	bCollisionEnabled = true;
	tint = Color::Red;
}

void Enemy::OnFormationStep(Vector2D newFormationOffset)
{
	formationOffset = newFormationOffset;
	animation.AdvanceFrame();
	sprite = resourceManager->GetSprite(animation.GetCurrentFrame().imagePath);
}

void Enemy::OnTick(size_t frameID)
{
	UpdateLocation(frameID);
}

void Enemy::OnEndPlay(EndPlayReason reason)
{
	if (reason == EndPlayReason::Destroyed)
	{
		assert(vfxSystem);
		vfxSystem->PlayEffect(std::make_unique<ExplosionEffect>(location));

		const float soundVolume = 1.f;
		PlaySnd(soundExplosion, soundVolume);
	}

	Actor::OnEndPlay(reason);
}

void Enemy::UpdateLocation(size_t frameID)
{
	animOffset = Vector2D(0.0f);

	Vector2D tmpLocation;
	tmpLocation = baseLocation + animOffset + formationOffset;

	if (tmpLocation.x - size.x / 2 >= 0 && tmpLocation.x + size.x / 2 <= gameWindow.width)
	{
		location.x = tmpLocation.x;
	}

	if (tmpLocation.y - size.y / 2 >= 0 && tmpLocation.y + size.y / 2 <= gameWindow.height)
	{
		location.y = tmpLocation.y;
	}
}