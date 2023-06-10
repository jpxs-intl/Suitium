#if _WIN32

#include <cstdio>
#include <cstdlib>
#include <Windows.h>

#include "Addresses.hpp"
#include "Hooks.hpp"
#include "Hotfixes.hpp"
#include "Version.hpp"

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    (void)instance; (void)reason; (void)reserved;

    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        {
            std::uintptr_t baseAddress = (std::uintptr_t)GetModuleHandle(NULL);
            if (!addresses::Map(baseAddress) || !hotfixes::Make(baseAddress))
            {
                char text[256];
                std::snprintf(text, sizeof(char[256]), "Executable is not compatible with this Suitium version!\nAre you sure this is %d%c?", SUITIUM_GAME_VERSION_NUMBER, SUITIUM_GAME_VERSION_PATCH);
                MessageBox(NULL, text, "Fatal Error", MB_OK | MB_ICONERROR);
                std::abort();
            }
            
            InstallHooks();
            break;
        }
    }

    return TRUE;
}

#endif