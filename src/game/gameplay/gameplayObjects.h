#pragma once

#include "../effects/gameEffects.h"
#include <fstream>
#include <array>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Object representating the bullet used by Player object
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Object managing the score and containing the target achievable score value
class ScoreSystem : public Object
{
public:
	void Construct();

	void SetScore(const int value) { actualScore = value; }
	int GetScore() const { return actualScore; }
	void AddScore(const int value) { actualScore += value; }
	int GetHighScore() const { return highScore; }
	
protected:
	void OnEndPlay(EndPlayReason reason) override;

	int LoadHighScore();
	void SaveHighScore(const int value);

	int actualScore = 0;
	int highScore = 0;

	static constexpr char* highScoreFolder = "cache";
	static constexpr char* highScoreFile = "highscore.txt";
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Represents the main player and exposes all the functions for its full work. 
// Works also as a bullet manager and orchestrate them
class Player : public Actor
{
public:
	void Construct(ResourceManager* resourceManager);

	void Draw() override;

	static constexpr int ammoCount = 10;
	std::array<Bullet, ammoCount>& GetAmmo() { return ammo; }

protected:
	void OnTick(size_t frameID) override;

	bool CheckShouldShoot();

	void UpdatePosition(size_t frameID);

	void ConstructAmmo(ResourceManager* resourceManager);

	void UpdateAmmo(size_t frameID);

	void Shoot();

	void DrawAmmo();

	std::array<Bullet, ammoCount> ammo;

	int bulletID = 0;
	int cooldownTimer = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Object representating the enemy playing against the Player object
class Enemy : public Actor
{
public:
	Enemy() {}

	Enemy(int _ID)
	{
		ID = _ID;
		SetSize(_ID);
		SetBaseLocation(_ID);
	}

	void Construct(ResourceManager* resourceManager, VFXSystem* vfxSystem);

	int GetID() const { return ID; }
	void SetID(int _ID) { ID = _ID; }

	void SetSize(const int seed)
	{
		const float targetSize = CalculateSize(seed);
		size = Vector2D(targetSize);
	}

	void SetBaseLocation(const int seed)
	{
		baseLocation.x = static_cast<float>((seed % 10) * 60 + StartX);
		baseLocation.y = static_cast<float>((seed / 10) * 60 + StartY);
	}

	Vector2D GetBaseLocation() const { return baseLocation; }

	void OnFormationStep(Vector2D newFormationOffset);

	int GetReward() const { return killReward; }

	void Shoot();

protected:
	void OnTick(size_t frameID) override;
	void OnEndPlay(EndPlayReason reason) override;

	void UpdateLocation(size_t frameID);

	bool IsOrbitPhaseActive(int value) const
	{
		return ((value >> 6) & 0x7) == 0x7;
	}

	bool IsDivePhaseActive(int value) const
	{
		return ((value >> 8) & 0xf) == 0xf;
	}

	float CalculateSize(int seed) const
	{
		static constexpr float BaseSize = 25.f;
		static constexpr int SizeVariation = 17;
		return (BaseSize + (seed) % SizeVariation);
	}

	VFXSystem* vfxSystem = nullptr;
	SoundHandler soundExplosion = nullptr;
	SpriteAnimation animation;
	ResourceManager* resourceManager = nullptr;

	int ID = 0;

	Vector2D baseLocation = Vector2D(0.0f);
	Vector2D animOffset = Vector2D(0.0f);
	Vector2D formationOffset = Vector2D(0.0f);

	static constexpr float StartX = 120.0f;
	static constexpr float StartY = 130.0f;

	int killReward = 10;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Orchestrating all the enemies
class EnemyManager : public Object
{
public:
	void Construct(ResourceManager* resourceManager, VFXSystem* vfxSystem);

	void Draw();

	auto& GetEnemies() { return enemies; }

	int GetMaxEnemyCount() const { return maxEnemyCount; };

	void UpdateFormationStepSize();

	int GetAliveEnemyCount() const;

protected:
	void OnBeginPlay() override;
	void OnTick(size_t frameID) override;
	void OnEndPlay(EndPlayReason reason) override;

	void UpdateFormation();
	bool HasReachedBoundary() const;

	VFXSystem* vfxSystem = nullptr;

	static constexpr int maxEnemyCount = 50;
	std::array<Enemy, maxEnemyCount> enemies;

	// absolute offset of all enemies' positions from their initial locations
	Vector2D formationOffset = Vector2D(0.0f);
	float xStepSize = 0.0f;
	const float yStepSize = 20.0f;
	const Vector2D xStepSizeRange = Vector2D(10.f, 40.f); // min, max
	
	float formationMovementDirection = 1.0f;
	size_t framesPerFormationStep = 40;
	const Vector2D framesPerFormationRange = Vector2D(10.f, 40.f); // max, min
};
