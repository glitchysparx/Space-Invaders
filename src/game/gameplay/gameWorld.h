#pragma once

#include "../ui/hud.h"
#include "gameplayObjects.h"


class BackgroundSound 
{
public:
	void Construct(ResourceManager* _resourceManager) 
	{
		resourceManager = _resourceManager;
	}

	void PlayVictory();
	void PlayGameover();
	void PlayGameplay();

protected:
	SoundHandler sound = nullptr;
	char* path = nullptr;
	float volume = 1.f;

	int musicHandle = 0;

	ResourceManager* resourceManager = nullptr;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Main manager orchestrating all the gameplay objects together with hud and background sound managers
class GameWorld : public Object
{
public:
	void Construct(ResourceManager* resourceManager);

	void Draw();

protected:
	void OnBeginPlay() override;
	void OnTick(size_t frameID) override;
	void OnEndPlay(EndPlayReason reason) override;

	void CheckCollisions();

	void CheckPlayerBulletsAgainstEnemies();

	void CheckEnemiesAgainstPlayer();

	// Describes and checks game rules
	void UpdateGameStatus();

	bool IsPlayerDead() const { return !player.IsAlive(); }

	bool AreEnemiesDefeated();

	void HandleStatusTransitions();

	void OnGameStarted();

	void OnVictory();

	void OnGameover();

	void TickGameplay(const size_t frameID);

	Player player;
	EnemyManager enemyManager;
	ScoreSystem scoreSystem;
	HUD hud;
	VFXSystem vfxSystem;

	GameStatus status;
	BackgroundSound backgroundSound;
};
