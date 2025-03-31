#pragma once

enum GameType {
	Shenmue_1 = 1,
	Shenmue_2 = 2,
	Invalid = 0xFF
};

extern GameType GGameType;
extern uintptr_t GetBaseAddress();

extern bool bDisableFileLog;

extern void DebugLog(const char* format, ...);
extern void DebugStop();
extern GameType DetectVersion(GameType OfType = Invalid);

#if defined(__cplusplus) && __cplusplus >= 202002L
#   define LOG(fmt, ...)           DebugLog("[{}] " fmt, __FUNCTION__, ##__VA_ARGS__)
#else
#   define Log(fmt, ...)           DebugLog("[%s] " fmt, __func__, ##__VA_ARGS__)
#endif
