// LemonHaze - 2025
#pragma once

// @brief Set to 1 to halt execution after all hooks have been setup
#define HOOK_CHECK		1

// @brief Set to 1 to exit after halt, if HOOK_CHECK is enabled
#define HOOK_CHECK_EXIT	0

// @brief Set to 1 to force disable logging
#define NO_LOGGING		0

// @brief Set to 1 to enable logging in Release config
#define LOG_IN_RELEASE	1

// @brief Set to 1 to compile as an executable, 0 to compile as a DLL and hook all functions startup.
#define STATIC_BUILD	0

// @brief Should we compile our own main?
#define COMPILE_MAIN	1

// @brief Shenmue 1
#define SHENMUE_1		1
#define SHENMUE_1_NAME	"Coconut"

// @brief Shenmue 2
#define SHENMUE_2		2
#define SHENMUE_2_NAME	"Mango"

// @brief Which game to build, SHENMUE_1 or SHENMUE_2
#define BUILD			SHENMUE_2

// @brief Should we log to file at all?
constexpr bool LOG_TO_FILE = true;

// @brief Log output filename
constexpr const char* LOG_FILE_NAME = "log.txt";

// ============================================================================

#ifndef BUILD
#	error "No build type specified"
#endif

#ifndef _MSC_VER
#	error "Only MSVC supported"
#endif


#define _CRT_SECURE_NO_WARNINGS