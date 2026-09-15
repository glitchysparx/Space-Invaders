#include "lib/game/gameplayObjects/gameWorld.h"
#include <math.h>
#include <windows.h>

void Game()
{
	// ===== Load resourses and init instances ===== 

	ResourceManager resourceManager;

	GameWorld gameWorld;
	gameWorld.Construct(&resourceManager);

	gameWorld.BeginPlay();

	// ===== Main Game loop ===== 

	while (true) 
	{
		++gameWindow.frameID;

		if (WantClose())
		{
			gameWorld.EndPlay(EndPlayReason::GameShutdown);
			return;
		}

		// ===== Tick ===== 

		gameWorld.Tick(gameWindow.frameID);

		// ===== Draw ===== 

		gameWorld.Draw();

		// ===== End of frame processing =====

		Flip();
	}
}
