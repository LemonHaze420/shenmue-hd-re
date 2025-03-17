#pragma once

#include "shenmue.h"

#include <Windows.h>
#include <cstdio>
#include <ctime>
#include <cstdarg>

#include <stdio.h>
#include <MinHook.h>

enum GameType {
	Shenmue_1 = 1,
	Shenmue_2 = 2,
	Invalid = 0xFF
};

extern GameType GGameType;
extern uintptr_t GetBaseAddress();

extern void Log(const char* format, ...);
extern void DebugStop();
extern GameType DetectVersion(GameType OfType = Invalid);
