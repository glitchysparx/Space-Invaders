#pragma once

#include "../effects/gameEffects.h"
#include <fstream>
#include <array>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Object representating the bullet used by Player object
class Bullet : public Object
{
public:
	void Construct(ResourceManager* _resourceManager) override;

	void Tick(size_t frameID) override;

	void BeginPlay() override;

protected:
	SoundHandler soundShoot = nullptr;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Object managing the score and containing the target achievable score value
class ScoreSystem : public Object
{
public:
	void Construct(ResourceManager* _resourceManager) override;

	void Tick(size_t frameID) override {};
	void Draw() override {};
	void EndPlay(EndPlayReason reason) override;

	void SetScore(const int value) { actualScore = value; }
	int GetScore() const { return actualScore; }
	void AddScore(const int value) { actualScore += value; }
	int GetHighScore() const { return highScore; }
	
protected:
	int actualScore = 0;
	int highScore = 0;

	static constexpr char* highScoreFolder = "cache";
	static constexpr char* highScoreFile = "highscore.txt";

	int LoadHighScore();
	void SaveHighScore(const int value);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Represents the main player and exposes all the functions for its full work. 
// Works also as a bullet manager and orchestrate them
class Player : public Object
{
public:
	void Construct(ResourceManager* _resourceManager) override;

	void Tick(size_t frameID) override;

	void Draw() override;

	static constexpr int ammoCount = 10;
	std::array<Bullet, ammoCount>& GetAmmo() { return ammo; }

	void SetVFXSystem(VFXSystem* _vfxSystem) { vfxSystem = _vfxSystem; }

protected:
	std::array<Bullet, ammoCount> ammo;

	int bulletID = 0;
	int cooldownTimer = 0;

	VFXSystem* vfxSystem = nullptr;

	bool CheckShouldShoot();

	void UpdatePosition(float frameID);

	void LoadAmmo();

	void UpdateAmmo(float frameID);

	void Shoot();

	void DrawAmmo();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Object representating the enemy playing against the Player object
class Enemy : public Object
{
public:
	Enemy() {}

	Enemy(int _ID)
	{
		ID = _ID;
		SetSize(_ID);
		SetBaseLocation(_ID);
	}

	void Construct(ResourceManager* _resourceManager) override;
	void Tick(size_t frameID) override;
	void EndPlay(EndPlayReason reason) override;

	int GetID() const { return ID; }
	void SetID(int _ID) { ID = _ID; }

	void SetSize(const int seed)
	{
		const float targetSize = CalculateSize(seed);
		size = Vector2D(targetSize);
	}

	void SetBaseLocation(const int seed)
	{
		baseX = static_cast<float>((seed % 10) * 60 + StartX);
		baseY = static_cast<float>((seed / 10) * 60 + StartY);
	}

	int GetReward() const { return killReward; }

	void SetVFXSystem(VFXSystem* _vfxSystem) { vfxSystem = _vfxSystem; }

protected:
	VFXSystem* vfxSystem = nullptr;
	SoundHandler soundExplosion = nullptr;

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

	int ID = 0;

	float baseX = 0.0f;
	float baseY = 0.0f;

	float offsetX = 0.0f;
	float offsetY = 0.0f;

	static constexpr float StartX = 120.0f;
	static constexpr float StartY = 130.0f;

	int killReward = 10;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Orchestrating all the enemies
class EnemyManager : public Object
{
public:
	void Construct(ResourceManager* _resourceManager) override;

	void Tick(size_t frameID) override;

	void Draw() override;

	void BeginPlay() override;

	void EndPlay(EndPlayReason reason) override;

	auto& GetEnemies() { return enemies; }

	void DestroyEnemy(size_t ID)
	{
		enemies.at(ID).EndPlay(EndPlayReason::Destroyed);
	}

	int GetMaxEnemyCount() const { return maxEnemyCount; };

	void SetVFXSystem(VFXSystem* _vfxSystem);

protected:
	VFXSystem* vfxSystem = nullptr;

	static constexpr int maxEnemyCount = 50;
	std::array<Enemy, maxEnemyCount> enemies;
};
