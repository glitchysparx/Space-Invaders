#include "gameplayObjects.h"

void Bullet::Construct(ResourceManager* _resourceManager)
{
	if (!_resourceManager || sprite) { return; }
	Object::Construct(_resourceManager);

	sprite = resourceManager->GetSprite("gfx/characters/bullet.png");
	bCollisionEnabled = true;
	soundShoot = resourceManager->GetSound("sounds/sfx/laserShoot.wav", false);
	size = Vector2D(16.f);
	collisionSize = size;
}

void Bullet::Tick(size_t frameID)
{
	if (!IsAlive()) { return; }

	const float targetLocationY = location.y - 4;
	if (targetLocationY <= 100)
	{
		EndPlay(EndPlayReason::Destroyed);
	}

	location.y -= 4.f;
}

void Bullet::BeginPlay()
{
	if (IsAlive()) { return; }
	Object::BeginPlay();

	bCollisionEnabled = true;
	const float soundVolume = 1.f;
	PlaySnd(soundShoot, soundVolume);
}



void ScoreSystem::Construct(ResourceManager* _resourceManager)
{
	Object::Construct(_resourceManager);
	highScore = LoadHighScore();
}

void ScoreSystem::EndPlay(EndPlayReason reason)
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



void Player::Construct(ResourceManager* _resourceManager)
{
	if (!_resourceManager || sprite) { return; }
	Object::Construct(_resourceManager);

	sprite = resourceManager->GetSprite("gfx/characters/Big Invader.png");
	LoadAmmo();

	location = Vector2D(400.f, 550.f);
	size = Vector2D(64.f);

	bCollisionEnabled = true;
	collisionSize = size;
}

void Player::Tick(size_t frameID)
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
	Object::Draw();

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

void Player::UpdatePosition(float frameID)
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

void Player::LoadAmmo()
{
	for (Bullet& bullet : ammo)
	{
		bullet.Construct(resourceManager);
	}
}

void Player::UpdateAmmo(float frameID)
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



void Enemy::Construct(ResourceManager* _resourceManager)
{
	if (!_resourceManager || sprite) { return; }
	Object::Construct(_resourceManager);

	sprite = resourceManager->GetSprite("gfx/characters/Little Invader.png");
	soundExplosion = resourceManager->GetSound("sounds/sfx/collision.wav", false);

	bCollisionEnabled = true;

	tint = Color::Red;
}

void Enemy::Tick(size_t frameID)
{
	if (!IsAlive()) { return; }

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
	offsetX = 0.0f;
	offsetY = 0.0f;

	const int n1 = frameID + ID * ID + ID * ID * ID;
	const int n2 = frameID + ID + ID * ID + ID * ID * ID * 3;

	if (IsOrbitPhaseActive(n1))
	{
		offsetX += (1.0f - cosf((n1 & 0x7f) / 64.0f * 2.0f * PI)) * (20.0f + ((ID * ID) % 9));
		offsetY += sinf((n1 & 0x7f) / 64.0f * 2.0f * PI) * (20.0f + ((ID * ID) % 9));
	}

	if (IsDivePhaseActive(n2))
	{
		offsetY += (1.0f - cosf((n2 & 0xff) / 256.0f * 2.0f * PI)) * (150.0f + ((ID * ID) % 9));
	}

	location.x = baseX + offsetX;
	location.y = baseY + offsetY;
}



void EnemyManager::Construct(ResourceManager* _resourceManager)
{
	if (!_resourceManager || sprite) { return; }
	Object::Construct(_resourceManager);

	int ID = 0;
	for (Enemy& enemy : enemies)
	{
		enemy.Construct(resourceManager);
		enemy.SetID(ID);
		enemy.SetSize(ID);
		enemy.SetBaseLocation(ID);
		enemy.SetCollisionSize(enemy.GetSize());
		ID++;
	}
}

void EnemyManager::Tick(size_t frameID)
{
	if (!IsAlive()) { return; }

	for (Enemy& enemy : enemies)
	{
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

void EnemyManager::BeginPlay()
{
	if (IsAlive()) { return; }
	Object::BeginPlay();

	for (Enemy& enemy : enemies)
	{
		enemy.BeginPlay();
	}
}

void EnemyManager::EndPlay(EndPlayReason reason)
{
	if (!IsAlive()) { return; }

	for (Enemy& enemy : enemies)
	{
		enemy.EndPlay(reason);
	}

	Object::EndPlay(reason);
}

void EnemyManager::SetVFXSystem(VFXSystem* _vfxSystem)
{
	if (!_vfxSystem) { return; }

	vfxSystem = _vfxSystem;
	for (Enemy& enemy : enemies)
	{
		enemy.SetVFXSystem(vfxSystem);
	}
}
