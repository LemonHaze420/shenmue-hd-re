// LemonHaze - 2025
#include "pch.h"
#include "../shenmue.h"

DEFINE_VAR(__int64,         g_hlib_root_data_ptr);      
DEFINE_VAR(int,             HLibSize);                  
DEFINE_VAR(__int64,         g_hlib_root_data_ptr2);     

DEFINE_VAR(__int64,         g_pCurrentTaskData);
DEFINE_VAR(__int64,         g_hlib_last_data_ptr);
DEFINE_VAR(__int64[3],      g_hlib_end_ptrs);
DEFINE_VAR(HLib_Task*,      CurrentTask);
DEFINE_VAR(HLib_Task*,      TaskQueue_StartOfNextFree);
DEFINE_VAR(int,             g_HasIncrementedTask);
DEFINE_VAR(HLib_Task* [16], g_pModuleTaskQueue);


// Shenmue
DEFINE_VAR(char,    smallTextures);
DEFINE_VAR(char,    logLoadedAssets);
DEFINE_VAR(void*,   memPtr_logLoadedAssets);
DEFINE_VAR(int,     memPtr_logLoadedAssets_);
DEFINE_VAR(int,     memPtr_logLoadedAssets_Size_2);

// HLib
namespace HLib {
    extern "C" HLib_Task* __fastcall EnqueueTaskWithoutParameter(
        int64_t* callbackFunction,
        signed __int16 nextFunctionIndex,
        unsigned __int16 r8b,
        int32_t taskToken)
    {

        if (nextFunctionIndex >= 16)
            return 0LL;

        if (g_HasIncrementedTask())
            return 0LL;

        HLib_Task* iter_task = TaskQueue_StartOfNextFree();
        if (!TaskQueue_StartOfNextFree())
            return 0LL;

        TaskQueue_StartOfNextFree() = TaskQueue_StartOfNextFree()->next;
        HLib_Task* next_task_queue = g_pModuleTaskQueue()[nextFunctionIndex];
        HLib_Task** next_task = &g_pModuleTaskQueue()[nextFunctionIndex];
        HLib_Task* next_task_2 = next_task_queue->nextTask;
        iter_task->nextTask = next_task_2;
        iter_task->next = next_task_queue;
        next_task_queue->nextTask = iter_task;
        next_task_2->next = iter_task;
        HLib_Task* curr_task = CurrentTask();
        iter_task->taskIndex = nextFunctionIndex;
        iter_task->r8b = r8b;
        iter_task->taskCallbackFnPtr = (void(__fastcall*)(HLibTaskData*))callbackFunction;
        iter_task->FirstTask = curr_task;
        iter_task->someTaskPtr2 = 0LL;
        iter_task->someTaskPtr3 = 0LL;
        iter_task->initCallbackFnPtr = 0LL;
        iter_task->unk_taskCallbackFnPtr = 0LL;
        iter_task->task_data_ptr = 0LL;
        iter_task->someTaskPtr5 = 0LL;
        iter_task->taskToken = taskToken;
        if (nextFunctionIndex < 16LL)
        {
            do
            {
                if (*next_task == next_task_queue)
                    *next_task = iter_task;
                ++next_task;
            } while ((__int64)next_task < (__int64)&g_HasIncrementedTask());
        }
        HLib_Task* someTaskPtr3 = curr_task->someTaskPtr3;
        iter_task->someTaskPtr4 = someTaskPtr3;
        if (someTaskPtr3)
            curr_task->someTaskPtr3->someTaskPtr5 = iter_task;
        else
            curr_task->someTaskPtr2 = iter_task;
        HLib_Task* result = iter_task;
        curr_task->someTaskPtr3 = iter_task;
        return result;
    }

}

// Main
// ========================================================
typedef int(__fastcall* Initialization_t)(int, char*);
Initialization_t InitOrig;

int InitReimpl(int InArgc, char* InArgv)
{

    smallTextures() = 0;
    if (InArgc > 1)
    {
        bool enabled = 0;
        const char** arg_str = (const char**)(InArgv + 8);
        __int64 arg_idx = (unsigned int)(InArgc - 1);
        do
        {
            int cmp = strcmp(*arg_str, "-smallTextures");
            const char* v9 = *arg_str;
            if (!cmp)
                enabled = true;
            smallTextures() = enabled;
            if (!strcmp(v9, "-logLoadedAssets"))
            {
                logLoadedAssets() = 1;
                void* memPtr = malloc(1048576uLL);
                enabled = smallTextures();
                memPtr_logLoadedAssets() = memPtr;
                memPtr_logLoadedAssets_() = 0;
                memPtr_logLoadedAssets_Size_2() = 1048576;
            }


            // our additions
            if (!strcmp(v9, "-nolog"))
            {
                bDisableFileLog = true;
            }
            if (!strcmp(v9, "-windowed"))
            {
                bForcedWindowed = true;
            }

            ++arg_str;
            --arg_idx;
        } while (arg_idx);
    }
    Log("Arguments: logLoadedAssets: %d \tsmallTextures: %d\n", logLoadedAssets(), smallTextures());
    return 1;
}

int __fastcall InitHook(int InArgc, char* InArgv)
{
    // this will handle SteamAPI init for us, we're going to undo everything else that was done
    // so we'll check the ret first to ensure that was successful.
    int ret = InitOrig(InArgc, InArgv);
    if (ret != 0) 
    {
        return InitReimpl(InArgc, InArgv);
    }
    return ret;
}

#if STATIC_BUILD == 0
int __fastcall MainHook(int InArgc, char* InArgv)
{
    int ret = InitHook(InArgc, InArgv);
    if (ret) 
    {
        HLib_Task* taskPtr = nullptr;
#if 1
        for (taskPtr = *(HLib_Task**)&CurrentTask(); ; *(HLib_Task**)&CurrentTask() = taskPtr)
        {
            // call
            taskPtr->taskCallbackFnPtr(taskPtr->task_data_ptr);

            // move to next
            taskPtr = (*(HLib_Task**)&CurrentTask())->nextTask;
        }
#else
        taskPtr = *CurrentTaskPtr;
        *CurrentTaskPtr = taskPtr;
        while (true)
        {
            taskPtr->taskCallbackFnPtr(taskPtr->task_data_ptr);
            taskPtr = taskPtr->nextTask;
            *CurrentTaskPtr = taskPtr;
        }
#endif
    }
    return ret;
}
#endif


#if BUILD == SHENMUE_1
#   if STATIC_BUILD == 1 && COMPILE_MAIN == 1
        int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
        {
                int argc = 0;
                LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
                Main(argc, (char*)lpCmdLine);
                return 0;
        }
#   elif STATIC_BUILD == 0
#	include "../hooks.h"
        void Attach()
        {
                CreateDebugConsole();
                InstallHooks();
                DebugStop();
        }
        void Detach()
        {
                FreeConsole();
                RemoveHooks();
        }
        BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
        {
                DisableThreadLibraryCalls(hModule);
                GGameType = DetectVersion();
                if (GGameType == Invalid)
                    return TRUE;

                if (dwReason == DLL_PROCESS_ATTACH)
                        Attach();
                else if (dwReason == DLL_PROCESS_DETACH)
                        Detach();
                return TRUE;
        }
#   endif
#endif