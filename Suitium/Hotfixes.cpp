#include "Hotfixes.hpp"

#include <cstdint>

#include "Addresses.hpp"

#if _WIN32
#include <windows.h>
#endif

bool hotfixes::Make(std::uintptr_t baseAddress)
{
#if _WIN32
    // When a bullet hits a train, it skips the hit code
    // This makes the code jump directly into the hit code for the hull (ignores windows) 
    if (!*addresses::IsDedicated.ptr)
    {
        DWORD oldProtect;
        VirtualProtect((LPVOID)(baseAddress + 0x2CABD), 0x8, PAGE_EXECUTE_READWRITE, &oldProtect);

        *(std::uint8_t *)(baseAddress + 0x2CABD + 0x0) = 0x0F;
        *(std::uint8_t *)(baseAddress + 0x2CABD + 0x1) = 0x84;
        *(std::uint8_t *)(baseAddress + 0x2CABD + 0x2) = 0xD2;
        *(std::uint8_t *)(baseAddress + 0x2CABD + 0x3) = 0x00;
        *(std::uint8_t *)(baseAddress + 0x2CABD + 0x4) = 0x00;
        *(std::uint8_t *)(baseAddress + 0x2CABD + 0x5) = 0x00;

        VirtualProtect((LPVOID)(baseAddress + 0x2CABD), 0x8, oldProtect, &oldProtect);
    }
    else
    {
        // TODO:
    }

    return true;
#else
    return false; // TODO:
#endif
}