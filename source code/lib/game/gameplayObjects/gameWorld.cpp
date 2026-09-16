#include "gameWorld.h"


void GameWorld::Construct(ResourceManager* resourceManager)
{
	if (!resourceManager) { return; }

	backgroundSound.Construct(resourceManager);
	vfxSystem.Construct(resourceManager);

	player.Construct(resourceManager);
	enemyManager.Construct(resourceManager, &vfxSystem);
	scoreSystem.Construct();
	hud.Construct(resourceManager);

	// Cache the highest achieved score into HUD
	hud.SetHighScore(scoreSystem.GetHighScore());
}

void GameWorld::BeginPlay()
{
	if (IsAlive()) { return; }
	Object::BeginPlay();

	vfxSystem.BeginPlay();
	player.BeginPlay();
	enemyManager.BeginPlay();
	scoreSystem.BeginPlay();
	hud.BeginPlay();

	status.Set(GameStatus::Status::Playing);

	OnGameStarted();
}

void GameWorld::Tick(size_t frameID)
{
	// The previous tick is finished only at the moment when a new tick started
	status.EndTick();

	UpdateGameStatus();
	HandleStatusTransitions();

	TickGameplay(frameID);
}

void GameWorld::Draw()
{
	if (status.Is(GameStatus::Status::Playing))
	{
		// Draw all the objects for game process
		player.Draw();
		enemyManager.Draw();
		hud.Draw();
		vfxSystem.Draw();
		return;
	}

	// Print the final game result
	hud.DrawResult();
}

void GameWorld::EndPlay(EndPlayReason reason)
{
	player.EndPlay(reason);
	enemyManager.EndPlay(reason);
	scoreSystem.EndPlay(reason);
	hud.EndPlay(reason);
	vfxSystem.EndPlay(reason);
	Object::EndPlay(reason);
}

void GameWorld::CheckCollisions()
{
	// Collision detection is not complex for this game. All the sprites are alligned, there is no point to worry about rotations, 
	// so AABB (axis alligned bounding box) is enough. AABB collision detection is so simple but the most interesting question is 
	// filtering. What exact boxes should we check? There are 2 options:
	// 1. Brute force - check every bullet against every enemy each frame
	// 2. Smart filters - reduce the number of candidate pairs
	// For this game the brute force method should be enough. There are only 10 bullets and 50 enemies. 
	// So, 10 * 50 = 500 checks per frame. 500 checks/frame × 60 = 30,000 checks/second. 
	// For such trivial AABB overlap tests, that is nothing on modern hardware

	CheckPlayerBulletsAgainstEnemies();
	CheckEnemiesAgainstPlayer();
}

void GameWorld::CheckPlayerBulletsAgainstEnemies()
{
	auto& ammo = player.GetAmmo();
	auto& enemies = enemyManager.GetEnemies();

	// Check every bullet against every enemy
	for (Bullet& bullet : ammo)
	{
		if (!bullet.IsAlive()) { continue; }

		const AABB bulletBounds = bullet.GetBounds();

		for (Enemy& enemy : enemies)
		{
			if (!enemy.IsAlive()) { continue; }

			if (AABB::Intersects(bulletBounds, enemy.GetBounds()))
			{
				bullet.EndPlay(EndPlayReason::Destroyed);
				enemy.EndPlay(EndPlayReason::Destroyed);
				enemyManager.UpdateFormationSpeed();
				scoreSystem.AddScore(enemy.GetReward());
				hud.SetScore(scoreSystem.GetScore());
				break;
			}
		}
	}
}

void GameWorld::CheckEnemiesAgainstPlayer()
{
	if (!player.IsAlive()) { return; }

	const AABB playerBounds = player.GetBounds();
	auto& enemies = enemyManager.GetEnemies();

	// Check every enemy against player
	for (Enemy& enemy : enemies)
	{
		if (!enemy.IsAlive()) { continue; }

		if (AABB::Intersects(playerBounds, enemy.GetBounds()))
		{
			player.EndPlay(EndPlayReason::Destroyed);
			enemy.EndPlay(EndPlayReason::Destroyed);
			break;
		}
	}
}

void GameWorld::UpdateGameStatus()
{
	if (status.Is(GameStatus::Status::Playing))
	{
		if (IsPlayerDead())
		{
			status.Set(GameStatus::Status::GameOver);
		} 
		else if (AreEnemiesDefeated())
		{
			status.Set(GameStatus::Status::Victory);
		}
	}

	hud.SetStatus(status.Get());
}

bool GameWorld::AreEnemiesDefeated()
{
	auto& enemies = enemyManager.GetEnemies();
	for (const Enemy& enemy : enemies)
	{
		if (enemy.IsAlive()) { return false; }
	}
	return true;
}

void GameWorld::HandleStatusTransitions()
{
	if (status.WasChangedTo(GameStatus::Status::Victory))
	{
		OnVictory();
		return;
	}

	if (status.WasChangedTo(GameStatus::Status::GameOver))
	{
		OnGameover();
		return;
	}
}

void GameWorld::OnGameStarted()
{
	backgroundSound.PlayGameplay();
}

void GameWorld::OnVictory()
{
	backgroundSound.PlayVictory();
	hud.PrepareResult();

	player.EndPlay(EndPlayReason::GameShutdown);
	enemyManager.EndPlay(EndPlayReason::GameShutdown);
	vfxSystem.EndPlay(EndPlayReason::GameShutdown);
}

void GameWorld::OnGameover()
{
	backgroundSound.PlayGameover();
	hud.PrepareResult();

	player.EndPlay(EndPlayReason::GameShutdown);
	enemyManager.EndPlay(EndPlayReason::GameShutdown);
	vfxSystem.EndPlay(EndPlayReason::GameShutdown);
}

void GameWorld::TickGameplay(const size_t frameID)
{
	player.Tick(frameID);
	enemyManager.Tick(frameID);

	if (status.Is(GameStatus::Status::Playing)) 
	{ 
		CheckCollisions();
	}

	hud.Tick(frameID);
	vfxSystem.Tick(frameID);
}



void BackgroundSound::PlayVictory()
{
	if (!resourceManager)
	{
		assert(resourceManager);
		return;
	}

	SoundHandler sound = resourceManager->GetSound("sounds/sfx/victory.wav", false);
	StopMusic();
	volume = 1.f;
	musicHandle = PlaySnd(sound, volume);
}

void BackgroundSound::PlayGameover()
{
	if (!resourceManager)
	{
		assert(resourceManager);
		return;
	}

	SoundHandler sound = resourceManager->GetSound("sounds/sfx/gameover.wav", false);
	StopMusic();
	volume = 1.f;
	musicHandle = PlaySnd(sound, volume);
}

void BackgroundSound::PlayGameplay()
{
	path = "sounds/music/energetic-background-music.wav"; 
	volume = 0.7f;
	musicHandle = PlayMusic(path, volume);
}
