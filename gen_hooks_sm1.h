#pragma once
#include <MinHook.h>
#include "shenmue.h"
#include "Vector3.h"
#include "HLib.h"
#include "dx.h"

typedef struct {
    const char* name;
    uintptr_t address;
    void* hook;
    void** original;
} Hook;

//extern void Main(int argc, char* argv);

static Hook hooks[] = {
#   include "s1/main_hooks.h"
#   include "s1/dx_hooks.h"
#   include "Vector3_hooks.h"
};

#define HOOK_COUNT (sizeof(hooks) / sizeof(hooks[0]))