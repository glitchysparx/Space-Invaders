#include "BackgroundSound.h"
#include <cassert>

void BackgroundSound::PlayVictory()
{
	if (!resourceManager)
	{
		assert(resourceManager);
		return;
	}

	SoundHandler sound = resourceManager->GetSound("assets/sounds/sfx/victory.wav", false);
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

	SoundHandler sound = resourceManager->GetSound("assets/sounds/sfx/gameover.wav", false);
	StopMusic();
	volume = 1.f;
	musicHandle = PlaySnd(sound, volume);
}

void BackgroundSound::PlayGameplay()
{
	path = "assets/sounds/music/energetic-background-music.wav"; 
	volume = 0.7f;
	musicHandle = PlayMusic(path, volume);
}
