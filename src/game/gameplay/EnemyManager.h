#pragma once

#include <memory>

#include "../../framework/core/Object.h"

class Enemy;
class VFXSystem;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Orchestrating all the enemies
class EnemyManager : public Object
{
public:
	EnemyManager();
	~EnemyManager() override;

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
	std::array<std::unique_ptr<Enemy>, maxEnemyCount> enemies;

	// absolute offset of all enemies' positions from their initial locations
	Vector2D formationOffset = Vector2D(0.0f);
	float xStepSize = 0.0f;
	const float yStepSize = 20.0f;
	const Vector2D xStepSizeRange = Vector2D(10.f, 40.f); // min, max

	float formationMovementDirection = 1.0f;
	size_t framesPerFormationStep = 40;
	const Vector2D framesPerFormationRange = Vector2D(10.f, 40.f); // max, min
};
