#pragma once

#define DEBUG_ENABLED	(defined(_DEBUG) || (!defined(_DEBUG) && LOG_IN_RELEASE == 1))

#if DEBUG_ENABLED
#	include <Windows.h>
#	include <iostream>
#endif

static void CreateDebugConsole()
{
#	if DEBUG_ENABLED
		AllocConsole();
		FILE* f;
		freopen_s(&f, "CONOUT$", "w", stdout);
		freopen_s(&f, "CONOUT$", "w", stderr);
		freopen_s(&f, "CONIN$", "r", stdin);
#	endif
}
