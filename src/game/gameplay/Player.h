#pragma once

#include <memory>

#include "../../framework/core/Object.h"
//#include "Bullet.h"

class Bullet;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Represents the main player and exposes all the functions for its full work. 
// Works also as a bullet manager and orchestrate them
class Player : public Actor
{
public:
	Player(); 
	~Player() override;

	void Construct(ResourceManager* resourceManager);

	void Draw() override;

	static constexpr int ammoCount = 10;
	std::array<std::unique_ptr<Bullet>, ammoCount>& GetAmmo() { return ammo; }

protected:
	void OnTick(size_t frameID) override;

	bool CheckShouldShoot();

	void UpdatePosition(size_t frameID);

	void ConstructAmmo(ResourceManager* resourceManager);

	void UpdateAmmo(size_t frameID);

	void Shoot();

	void DrawAmmo();

	std::array <std::unique_ptr<Bullet>, ammoCount> ammo;

	int bulletID = 0;
	int cooldownTimer = 0;
};
