#pragma once

#include "../../framework/GameFramework.h"
#include <cstring>
#include <Windows.h>
#include <map>
#include <iostream>

using SpriteHandler = void*;
using SoundHandler = void*;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// color constants
namespace Color
{
	static constexpr DWORD White = 0xffffffff;
	static constexpr DWORD Black = 0xff000000;
	static constexpr DWORD Red = 0xffff0000;
	static constexpr DWORD Green = 0xff00ff00;
	static constexpr DWORD Blue = 0xff0000ff;
	static constexpr DWORD Yellow = 0xffffff00;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Vector2D
{
	float x = 0.0f;
	float y = 0.0f;

	Vector2D() = default;

	Vector2D(float value)
		: x(value), y(value) {}

	Vector2D(float xVal, float yVal)
		: x(xVal), y(yVal) {}

	Vector2D operator+(const Vector2D& other) const
	{
		return Vector2D(x + other.x, y + other.y);
	}

	Vector2D operator-(const Vector2D& other) const
	{
		return Vector2D(x - other.x, y - other.y);
	}

	Vector2D operator/(const float value) const
	{
		return Vector2D(x / value, y / value);
	}
};

bool IsVectorOnScreen(Vector2D value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// logs

// Helper function for logging string with float into the Output logs
void LogFloat(const char* label, float value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GameStatus 
{
public:
	enum class Status
	{
		Ready,
		Playing,
		Victory,
		GameOver
	};

	Status Get() const { return statusCurrent; }
	Status GetPrevious() const { return statusPrevious; }

	void Set(Status value);

	void EndTick() 
	{
		bIsStatusChanged = false;
	}

	bool Is(Status value) const 
	{
		return statusCurrent == value;
	}

	bool WasChangedTo(Status value) const
	{
		return bIsStatusChanged && statusCurrent == value;
	}

protected:
	bool bIsStatusChanged = false;
	Status statusCurrent = Status::Ready;
	Status statusPrevious = Status::Ready;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check whether the player asks for game exiting
bool WantClose();

