#pragma once
#include <MinHook.h>

typedef struct {
    const char* name;
    uintptr_t address;
    void* hook;
    void* original;
} Hook;

//extern void Main(int argc, char* argv);

static Hook hooks[] = {
    {}
};

#define HOOK_COUNT (sizeof(hooks) / sizeof(hooks[0]))
