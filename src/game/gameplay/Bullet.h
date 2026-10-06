#pragma once

#include "../../framework/core/Object.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Base class for bullets
class Bullet : public Actor
{
public:
	void Construct(ResourceManager* _resourceManager);

	void SetMovementStep(const float value) { movementStep = value; }

	void SetMovementDirection(const Vector2D value) { movementDirection = value; }

	void SetImage(const std::string path) { imagePath = path; }

protected:
	void OnTick(size_t frameID) override;
	void OnBeginPlay() override;

	SoundHandler soundShoot = nullptr;

	float movementStep = 4.f;
	Vector2D movementDirection = Vector2D(0.f, 1.f); // x-axis, y-axis

	std::string imagePath = "assets/gfx/characters/bullet.png";
	std::string soundPath = "assets/sounds/sfx/laserShoot.wav";
};
