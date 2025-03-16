// LemonHaze - 2025
#include "pch.h"
#include "../utilities.h"
#include "../debug.h"


#if BUILD == SHENMUE_2
#include "../hooks.h"

#   if STATIC_BUILD == 1 && COMPILE_MAIN == 1
        int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
        {
                int argc = 0;
                LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
                Main(argc, (char*)lpCmdLine);
                return 0;
        }
#   elif STATIC_BUILD == 0
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