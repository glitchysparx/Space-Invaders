#include "gameplayObjects.h"
#include <algorithm>

void Bullet::Construct(ResourceManager* resourceManager)
{
	if (!resourceManager || sprite) { return; }

	sprite = resourceManager->GetSprite("gfx/characters/bullet.png");
	soundShoot = resourceManager->GetSound("sounds/sfx/laserShoot.wav", false);

	bCollisionEnabled = true;
	size = Vector2D(16.f);
	collisionSize = size;
}

void Bullet::OnTick(size_t frameID)
{
	if (!IsAlive()) { return; }

	const float targetLocationY = location.y - 4;
	if (targetLocationY <= 100)
	{
		EndPlay(EndPlayReason::Destroyed);
	}

	location.y -= 4.f;
}

void Bullet::OnBeginPlay()
{
	if (IsAlive()) { return; }
	Object::BeginPlay();

	bCollisionEnabled = true;
	const float soundVolume = 1.f;
	PlaySnd(soundShoot, soundVolume);
}



void ScoreSystem::Construct()
{
	highScore = LoadHighScore();
}

void ScoreSystem::OnEndPlay(EndPlayReason reason)
{
	// To cache the actual score value if it is higher than previous score
	int lastHighScore = LoadHighScore();
	if (lastHighScore < actualScore)
	{
		SaveHighScore(actualScore);
	}

	Object::EndPlay(reason);
}

int ScoreSystem::LoadHighScore()
{
	std::string fullPath = std::string(highScoreFolder) + "/" + highScoreFile;

	std::ifstream file(fullPath);

	// Verify file exists
	if (!file.is_open()) { return 0; }

	int highScore = 0;
	file >> highScore;

	// Verify file contains valid data
	if (file.fail()) { return 0; }

	return highScore;
}

void ScoreSystem::SaveHighScore(const int value)
{
	if (value < 0) { return; }

	// 1. Automatically create the directory if it doesn't exist
	CreateDirectoryA(highScoreFolder, NULL);

	// 2. Combine folder and file name
	std::string fullPath = std::string(highScoreFolder) + "/" + highScoreFile;

	// 3. Now opening the file
	std::ofstream file(fullPath, std::ios::trunc);

	// Handle error
	if (!file.is_open()) { return; }

	file << value;
}



void Player::Construct(ResourceManager* resourceManager)
{
	if (!resourceManager || sprite) { return; }

	sprite = resourceManager->GetSprite("gfx/characters/Big Invader.png");

	ConstructAmmo(resourceManager);

	location = Vector2D(400.f, 550.f);
	size = Vector2D(64.f);

	bCollisionEnabled = true;
	collisionSize = size;
}

void Player::OnTick(size_t frameID)
{
	if (!IsAlive()) { return; }

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
	for (Bullet& bullet : ammo)
	{
		bullet.Construct(resourceManager);
	}
}

void Player::UpdateAmmo(size_t frameID)
{
	for (Bullet& bullet : ammo)
	{
		bullet.Tick(frameID);
	}
}

void Player::Shoot()
{
	Bullet& targetBullet = ammo[bulletID];
	targetBullet.SetLocation(location);
	targetBullet.BeginPlay();
	bulletID = (bulletID + 1) % ammoCount;
}

void Player::DrawAmmo()
{
	for (Bullet& bullet : ammo)
	{
		bullet.Draw();
	}
}



void Enemy::Construct(ResourceManager* resourceManager, VFXSystem* vfxSystem)
{
	if (!resourceManager || sprite) { return; }

	this->vfxSystem = vfxSystem;

	sprite = resourceManager->GetSprite("gfx/characters/Little Invader.png");
	soundExplosion = resourceManager->GetSound("sounds/sfx/collision.wav", false);

	bCollisionEnabled = true;
	tint = Color::Red;
}

void Enemy::OnTick(size_t frameID)
{
	UpdateLocation(frameID);
}

void Enemy::EndPlay(EndPlayReason reason)
{
	if (!IsAlive()) { return; }

	if (reason == EndPlayReason::Destroyed) 
	{ 
		assert(vfxSystem);
		vfxSystem->PlayEffect(std::make_unique<ExplosionEffect>(location));

		const float soundVolume = 1.f;
		PlaySnd(soundExplosion, soundVolume);
	}

	Object::EndPlay(reason);
}

void Enemy::UpdateLocation(size_t frameID)
{
	animOffset = Vector2D(0.0f);

	const int n1 = frameID + ID * ID + ID * ID * ID;
	const int n2 = frameID + ID + ID * ID + ID * ID * ID * 3;

	if (IsOrbitPhaseActive(n1))
	{
		animOffset.x += (1.0f - cosf((n1 & 0x7f) / 64.0f * 2.0f * PI)) * (20.0f + ((ID * ID) % 9));
		animOffset.y += sinf((n1 & 0x7f) / 64.0f * 2.0f * PI) * (20.0f + ((ID * ID) % 9));
	}

	if (IsDivePhaseActive(n2))
	{
		animOffset.y += (1.0f - cosf((n2 & 0xff) / 256.0f * 2.0f * PI)) * (150.0f + ((ID * ID) % 9));
	}

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



void EnemyManager::Construct(ResourceManager* resourceManager, VFXSystem* vfxSystem)
{
	if (!resourceManager) { return; }

	this->vfxSystem = vfxSystem;

	int ID = 0;
	for (Enemy& enemy : enemies)
	{
		enemy.Construct(resourceManager, vfxSystem);
		enemy.SetID(ID);
		enemy.SetSize(ID);
		enemy.SetBaseLocation(ID);
		enemy.SetCollisionSize(enemy.GetSize());
		ID++;
	}
}

void EnemyManager::OnTick(size_t frameID)
{
	UpdateFormation();

	for (Enemy& enemy : enemies)
	{
		enemy.SetFormationOffset(formationOffset);
		enemy.Tick(frameID);
	}
}

void EnemyManager::Draw()
{
	if (!IsAlive()) { return; }

	for (Enemy& enemy : enemies)
	{
		enemy.Draw();
	}
}

void EnemyManager::OnBeginPlay()
{
	Object::BeginPlay();

	for (Enemy& enemy : enemies)
	{
		enemy.BeginPlay();
	}

	UpdateFormationStepSize();
}

void EnemyManager::OnEndPlay(EndPlayReason reason)
{
	for (Enemy& enemy : enemies)
	{
		enemy.EndPlay(reason);
	}

	Object::EndPlay(reason);
}

void EnemyManager::UpdateFormationStepSize()
{
	float aliveRatio = static_cast<float>(GetAliveEnemyCount()) / static_cast<float>(maxEnemyCount);

	xStepSize = xStepSizeRange.y - (xStepSizeRange.y - xStepSizeRange.x) * aliveRatio;
}

int EnemyManager::GetAliveEnemyCount() const
{
	if (!IsAlive()) { return 0; }

	int count = 0;
	for (const Enemy& enemy : enemies)
	{
		count += enemy.IsAlive() ? 1 : 0;
	}
	return count;
}

void EnemyManager::UpdateFormation()
{
	formationOffset.x += xStepSize * movementDirection;

	if (HasReachedBoundary())
	{
		movementDirection *= -1;
		formationOffset.y += yStepSize;
	}
}

bool EnemyManager::HasReachedBoundary() const
{
	float minX = FLT_MAX;
	float maxX = -FLT_MAX;

	for (const Enemy& enemy : enemies)
	{
		if (!enemy.IsAlive()) { continue; }

		const float x = enemy.GetBaseLocation().x + formationOffset.x;

		minX = (std::min)(minX, x - enemy.GetSize().x / 2.0f);
		maxX = (std::max)(maxX, x + enemy.GetSize().x / 2.0f);
	}

	return minX <= 0 || maxX >= gameWindow.width;
}

