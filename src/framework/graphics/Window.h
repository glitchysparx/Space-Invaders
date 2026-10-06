#pragma once

#include "../core/Vector2D.h"
#include <cstddef>

struct Window
{
	int width = 800;
	int height = 600;
	const char name[22] = "Space Invaders Remake";

	std::size_t frameID = 0;
};

extern Window gameWindow;

// Check whether a point lies within the window bounds.
bool IsPointOnScreen(Vector2D point);
