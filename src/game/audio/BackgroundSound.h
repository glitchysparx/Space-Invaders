#pragma once

#include "../../framework/core/ResourceManager.h"

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
