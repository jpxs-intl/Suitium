#include "Hotfixes.hpp"

#include <cstdint>
#include <cstring>

#include "Addresses.hpp"

#if _WIN32
#include <windows.h>
#endif

#define NOP(address, size) std::memset(address, 0x90, size);

bool hotfixes::Make(std::uintptr_t baseAddress)
{
#if _WIN32
    // This is needed for custom menus, it allows new menu IDs to exist
    if (!*addresses::IsDedicated.ptr)
    {
        DWORD oldProtect;
        VirtualProtect((LPVOID)(baseAddress + 0x96C2E), 7, PAGE_EXECUTE_READWRITE, &oldProtect);
        NOP((std::uint8_t *)(baseAddress + 0x96C2E), 7);
        VirtualProtect((LPVOID)(baseAddress + 0x96C2E), 7, oldProtect, &oldProtect);
    }

    // When a bullet hits a train, it skips the hit code
    // This makes the code jump directly into the hit code for the hull (ignores windows) 
    if (!*addresses::IsDedicated.ptr)
    {
        DWORD oldProtect;
        VirtualProtect((LPVOID)(baseAddress + 0x2CABD), 5, PAGE_EXECUTE_READWRITE, &oldProtect);
        *(std::uint8_t *)(baseAddress + 0x2CABD + 0) = 0x0F;
        *(std::uint8_t *)(baseAddress + 0x2CABD + 1) = 0x84;
        *(std::uint8_t *)(baseAddress + 0x2CABD + 2) = 0xD2;
        *(std::uint8_t *)(baseAddress + 0x2CABD + 3) = 0x00;
        *(std::uint8_t *)(baseAddress + 0x2CABD + 4) = 0x00;
        *(std::uint8_t *)(baseAddress + 0x2CABD + 5) = 0x00;
        VirtualProtect((LPVOID)(baseAddress + 0x2CABD), 5, oldProtect, &oldProtect);
    }
    else
    {
        DWORD oldProtect;
        VirtualProtect((LPVOID)(baseAddress + 0x1F9A6), 5, PAGE_EXECUTE_READWRITE, &oldProtect);
        *(std::uint8_t *)(baseAddress + 0x1F9A6 + 0) = 0x0F;
        *(std::uint8_t *)(baseAddress + 0x1F9A6 + 1) = 0x84;
        *(std::uint8_t *)(baseAddress + 0x1F9A6 + 2) = 0xD2;
        *(std::uint8_t *)(baseAddress + 0x1F9A6 + 3) = 0x00;
        *(std::uint8_t *)(baseAddress + 0x1F9A6 + 4) = 0x00;
        *(std::uint8_t *)(baseAddress + 0x1F9A6 + 5) = 0x00;
        VirtualProtect((LPVOID)(baseAddress + 0x1F9A6), 5, oldProtect, &oldProtect);
    }

    return true;
#elif __linux__
    return true; // TODO: true for testing
#endif
}