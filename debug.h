#pragma once

#if _DEBUG
#	include <Windows.h>
#	include <iostream>
#endif

static void CreateDebugConsole()
{
#	if _DEBUG
		AllocConsole();
		FILE* f;
		freopen_s(&f, "CONOUT$", "w", stdout);
		freopen_s(&f, "CONOUT$", "w", stderr);
		freopen_s(&f, "CONIN$", "r", stdin);
#	endif
}
