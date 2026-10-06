#pragma once

#include "GameStatus.h"
#include "../audio/BackgroundSound.h"

#include "Player.h"
#include "../ui/HUD.h"
#include "ScoreSystem.h"
#include "EnemyManager.h"
#include "../effects/VFXSystem.h"


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
