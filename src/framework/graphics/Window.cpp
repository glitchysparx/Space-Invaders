#include "Window.h"

Window gameWindow;

bool IsPointOnScreen(Vector2D point)
{
	const bool bIsXValid = (point.x >= 0) && (point.x <= gameWindow.width);
	const bool bIsYValid = (point.y >= 0) && (point.y <= gameWindow.height);

	return bIsXValid && bIsYValid;
}
