#pragma once

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

constexpr bool LOG_TO_FILE = true;
constexpr const char* LOG_FILE_NAME = "log.txt";


extern void Log(const char* format, ...);
extern void DebugStop();
extern GameType DetectVersion(GameType OfType = Invalid);
