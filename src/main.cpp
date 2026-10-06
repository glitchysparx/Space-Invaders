#include "game/gameplay/GameWorld.h"
#include <math.h>
#include <windows.h>

// Check whether the player asks for game exiting
static bool WantClose()
{
	if (WantQuit()) { return true; }
	return IsKeyDown(VK_ESCAPE);
}

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
