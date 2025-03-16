// LemonHaze - 2025
#pragma once

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

#define BUILD			SHENMUE_2

// @brief Which game to build, SHENMUE_1 or SHENMUE_2
#ifndef BUILD
#	error "No build type specified"
#endif

#ifndef _MSC_VER
#	error "Only MSVC supported"
#endif

#define _CRT_SECURE_NO_WARNINGS