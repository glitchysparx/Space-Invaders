#include "Debug.h"
#include <cstdio>
#include <windows.h>

void LogFloat(const char* label, float value)
{
	char buffer[128];
	sprintf_s(buffer, "%s%f\n", label, value);
	OutputDebugStringA(buffer);
}
