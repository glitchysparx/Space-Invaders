#include "EnemyManager.h"
#include "Enemy.h"

EnemyManager::EnemyManager() = default;
EnemyManager::~EnemyManager() = default;
// default constructor and destructor are used because the class contains 
// std::array of std::unique_ptr<enemy>, which will be automatically 
// managed by the compiler if "Enemy.h" is included in the file

void EnemyManager::Construct(ResourceManager* resourceManager, VFXSystem* vfxSystem)
{
	if (!resourceManager) { return; }

	this->vfxSystem = vfxSystem;

	int ID = 0;
	for (auto& enemy : enemies)
	{
		enemy = std::make_unique<Enemy>();
		enemy->Construct(resourceManager, vfxSystem);
		enemy->SetID(ID);
		enemy->SetSize(ID);
		enemy->SetBaseLocation(ID);
		enemy->SetCollisionSize(enemy->GetSize());
		ID++;
	}
}

void EnemyManager::OnTick(size_t frameID)
{
	// Update formation every n-th frame
	if (frameID % framesPerFormationStep == 0)
	{
		UpdateFormation();
	}

	// Regularly tick all enemies
	for (auto& enemy : enemies)
	{
		enemy->Tick(frameID);
	}
}

void EnemyManager::Draw()
{
	if (!IsAlive()) { return; }

	for (auto& enemy : enemies)
	{
		enemy->Draw();
	}
}

void EnemyManager::OnBeginPlay()
{
	for (auto& enemy : enemies)
	{
		enemy->BeginPlay();
	}

	UpdateFormationStepSize();
}

void EnemyManager::OnEndPlay(EndPlayReason reason)
{
	for (auto& enemy : enemies)
	{
		enemy->EndPlay(reason);
	}
}

void EnemyManager::UpdateFormationStepSize()
{
	float aliveRatio = static_cast<float>(GetAliveEnemyCount()) / static_cast<float>(maxEnemyCount);

	xStepSize = xStepSizeRange.y - (xStepSizeRange.y - xStepSizeRange.x) * aliveRatio;

	framesPerFormationStep = static_cast<size_t>(framesPerFormationRange.x +
		(framesPerFormationRange.y - framesPerFormationRange.x) * aliveRatio);
}

int EnemyManager::GetAliveEnemyCount() const
{
	if (!IsAlive()) { return 0; }

	int count = 0;
	for (const auto& enemy : enemies)
	{
		count += enemy->IsAlive() ? 1 : 0;
	}
	return count;
}

void EnemyManager::UpdateFormation()
{
	formationOffset.x += xStepSize * formationMovementDirection;

	if (HasReachedBoundary())
	{
		formationMovementDirection *= -1;
		formationOffset.y += yStepSize;
	}

	for (auto& enemy : enemies)
	{
		if (!enemy->IsAlive()) { continue; }

		enemy->OnFormationStep(formationOffset);
	}
}

bool EnemyManager::HasReachedBoundary() const
{
	float minX = FLT_MAX;
	float maxX = -FLT_MAX;

	for (const auto& enemy : enemies)
	{
		if (!enemy->IsAlive()) { continue; }

		const float x = enemy->GetBaseLocation().x + formationOffset.x;

		minX = (std::min)(minX, x - enemy->GetSize().x / 2.0f);
		maxX = (std::max)(maxX, x + enemy->GetSize().x / 2.0f);
	}

	return minX <= 0 || maxX >= gameWindow.width;
}