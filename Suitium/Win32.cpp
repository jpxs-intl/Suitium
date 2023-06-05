#if _WIN32

#include <cstdio>
#include <cstdlib>
#include <Windows.h>

#include "Addresses.hpp"
#include "Hooks.hpp"
#include "Version.hpp"

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    (void)instance; (void)reason; (void)reserved;

    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        {
            if (!MapAddressesForWin32((std::uintptr_t)GetModuleHandle(NULL)))
            {
                char text[256];
                std::snprintf(text, sizeof(char[256]), "Executable is not compatible with this Suitium version!\nAre you sure this is %d%c?", GameVersionNumber, GameVersionPatch);
                MessageBox(NULL, text, "Fatal Error", MB_OK | MB_ICONERROR);
                std::abort();
            }

            PrepareHooks();
            break;
        }
    }

    return TRUE;
}

#endif