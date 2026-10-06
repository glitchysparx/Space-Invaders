#include "ResourceManager.h"

SpriteHandler ResourceManager::GetSprite(const std::string& path)
{
	// Check whether the target sprite was already loaded previously
	auto it = sprites.find(path);
	if (it != sprites.end())
	{
		return it->second;
	}

	// Load sprite by path
	SpriteHandler targetSprite = LoadSprite(path.c_str());
	if (!targetSprite)
	{
		OutputDebugStringA(("Failed to load sprite: " + path + "\n").c_str());
		return nullptr;
	}

	// Cache target sprite for future quick search
	sprites[path] = targetSprite;
	return targetSprite;
}

SoundHandler ResourceManager::GetSound(const std::string& path, bool looped)
{
	// Each sound sample can be loaded in 2 states: looped and not looped
	const std::string tag = path + "_" + (looped ? "1" : "0");

	// Check whether the target sound was already loaded previously
	auto it = sounds.find(tag);
	if (it != sounds.end())
	{
		return it->second;
	}

	// Load sound by path
	SoundHandler targetSound = LoadSnd(path.c_str(), looped);
	if (!targetSound)
	{
		OutputDebugStringA(("Failed to load sound: " + path + "\n").c_str());
		return nullptr;
	}

	// Cache target sound for future quick search
	sounds[tag] = targetSound;
	return targetSound;
}