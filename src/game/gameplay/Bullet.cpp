#include "Bullet.h"

void Bullet::Construct(ResourceManager* resourceManager)
{
	if (!resourceManager || sprite) { return; }

	sprite = resourceManager->GetSprite(imagePath);
	soundShoot = resourceManager->GetSound(soundPath, false);

	bCollisionEnabled = true;
	size = Vector2D(16.f);
	collisionSize = size;
}

void Bullet::OnTick(size_t frameID)
{
	const Vector2D targetLocation = location - movementDirection * movementStep;

	// Check whether the desired location is located on the rendered part of the screen 
	if (!IsFullyOnScreen(targetLocation))
	{
		EndPlay(EndPlayReason::Destroyed);
		return;
	}

	location = targetLocation;
}

void Bullet::OnBeginPlay()
{
	bCollisionEnabled = true;
	const float soundVolume = 1.f;
	PlaySnd(soundShoot, soundVolume);
}
