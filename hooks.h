// LemonHaze - 2025
#pragma once
#include "shenmue.h"

#if STATIC_BUILD == 0
#   if BUILD == SHENMUE_1
#       include "gen_hooks_sm1.h"
#   elif BUILD == SHENMUE_2
#       include "gen_hooks_sm2.h"
#   endif

void InstallHooks() {
    if (MH_Initialize() != MH_OK)
        return;

    for (size_t i = 0; i < HOOK_COUNT; i++) {
        void* target_address = (void*)(GetBaseAddress() + hooks[i].address);
        if (MH_CreateHook(target_address, hooks[i].hook, hooks[i].original) != MH_OK) {
            Log("Failed to create hook for %s\n", hooks[i].name);
        }
        else {
#           if _DEBUG
                //Log("Installed hook for %s\n", hooks[i].name);
#           endif
        }
    }


    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
        Log("Failed to enable hooks\n");
        return;
    }
    Log("Hook count = %lld\n", HOOK_COUNT);
}

void RemoveHooks() {
    if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK) {
        Log("Failed to disable hooks\n");
        return;
    }

    for (size_t i = 0; i < HOOK_COUNT; i++) {
        void* target_address = (void*)(GetBaseAddress() + hooks[i].address);
        if (MH_RemoveHook(target_address) != MH_OK) {
            Log("Failed to remove hook for %s\n", hooks[i].name);
        }
    }
    MH_Uninitialize();
}
#endif