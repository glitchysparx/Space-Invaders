#pragma once

#include "../GameFramework.h"
#include <map>
#include <string>
#include <Windows.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using SpriteHandler = void*;
using SoundHandler = void*;

// Stores all the resources and provides efficient global resource management
class ResourceManager
{
public:
	SpriteHandler GetSprite(const std::string& path);
	SoundHandler GetSound(const std::string& path, bool looped);

private:
	std::map<std::string, SpriteHandler> sprites;
	std::map<std::string, SoundHandler> sounds;
};