#include "GameStatus.h"

void GameStatus::Set(Status value)
{
	if (statusCurrent == value) { return; }

	statusPrevious = statusCurrent;
	statusCurrent = value;
	bIsStatusChanged = true;
}
