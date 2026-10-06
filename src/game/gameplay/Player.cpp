#include "Player.h"
#include "Bullet.h"
#include "../../framework/core/Debug.h"

Player::Player() = default; 
Player::~Player() = default;
// default constructor and destructor are used because the class contains 
// std::array of std::unique_ptr<bullet>, which will be automatically 
// managed by the compiler if "Bullet.h" is included in the file

void Player::Construct(ResourceManager* resourceManager)
{
	if (!resourceManager || sprite) { return; }

	sprite = resourceManager->GetSprite("assets/gfx/characters/Big Invader.png");

	ConstructAmmo(resourceManager);

	location = Vector2D(400.f, 550.f);
	size = Vector2D(64.f);

	bCollisionEnabled = true;
	collisionSize = size;
}

void Player::OnTick(size_t frameID)
{
	UpdatePosition(frameID);

	if (CheckShouldShoot())
	{
		Shoot();
	}

	UpdateAmmo(frameID);
}

void Player::Draw()
{
	if (!IsAlive()) { return; }
	Actor::Draw();

	DrawAmmo();
}

bool Player::CheckShouldShoot()
{
	if (!IsKeyDown(VK_SPACE))
	{
		cooldownTimer = 0;
		return false;
	}

	if (cooldownTimer == 0)
	{
		cooldownTimer = 15;
		return true;
	}

	if (cooldownTimer > 0)
	{
		--cooldownTimer;
	}
	return false;
}

void Player::UpdatePosition(size_t frameID)
{
	Vector2D targetLocation = location;
	targetLocation.x += IsKeyDown(VK_LEFT) ? -7 : IsKeyDown(VK_RIGHT) ? 7 : 0;
	//LogFloat("target location X = ", targetLocation.x);

	// Check whether the desired location is located on the rendered part of the screen 
	if (IsFullyOnScreen(targetLocation))
	{
		location = targetLocation;
	}

	angle = sin(frameID * 0.1) * 0.1;
}

void Player::ConstructAmmo(ResourceManager* resourceManager)
{
	for (auto& bullet : ammo)
	{
		bullet = std::make_unique<Bullet>();
		bullet->Construct(resourceManager);
	}
}

void Player::UpdateAmmo(size_t frameID)
{
	for (auto& bullet : ammo)
	{
		bullet->Tick(frameID);
	}
}

void Player::Shoot()
{
	auto& targetBullet = ammo[bulletID];
	targetBullet->SetLocation(location);
	targetBullet->BeginPlay();
	bulletID = (bulletID + 1) % ammoCount;
}

void Player::DrawAmmo()
{
	for (auto& bullet : ammo)
	{
		bullet->Draw();
	}
}