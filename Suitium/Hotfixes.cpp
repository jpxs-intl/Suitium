#include "Hotfixes.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "Addresses.hpp"

#if _WIN32
#include <windows.h>
#elif __linux__
#include <sys/mman.h>
#include <unistd.h>
#endif

#if __linux__    
int VirtualProtect(void *address, std::size_t size, int protection, int *oldProtection)
{
    long pagesize = sysconf(_SC_PAGESIZE);
    void *alignedAddress = (void *)((std::uintptr_t)address & ~(pagesize - 1));

    int prot = PROT_NONE;
    if (protection == PAGE_READONLY)
        prot = PROT_READ;
    if (protection == PAGE_READWRITE)
        prot = PROT_READ | PROT_WRITE;
    if (protection == PAGE_EXECUTE)
        prot = PROT_EXEC;
    if (protection == PAGE_EXECUTE_READ)
        prot = PROT_READ | PROT_EXEC;
    if (protection == PAGE_EXECUTE_READWRITE)
        prot = PROT_READ | PROT_WRITE | PROT_EXEC;

    mprotect(alignedAddress, pagesize, prot);

    if (oldProtection != nullptr)
        *oldProtection = protection;

    return 1;
}
#endif

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
        VirtualProtect((LPVOID)(baseAddress + 0x2CABD), 6, PAGE_EXECUTE_READWRITE, &oldProtect);
        *(std::uint8_t *)(baseAddress + 0x2CABD + 0) = 0x0F;
        *(std::uint8_t *)(baseAddress + 0x2CABD + 1) = 0x84;
        *(std::uint8_t *)(baseAddress + 0x2CABD + 2) = 0xD2;
        *(std::uint8_t *)(baseAddress + 0x2CABD + 3) = 0x00;
        *(std::uint8_t *)(baseAddress + 0x2CABD + 4) = 0x00;
        *(std::uint8_t *)(baseAddress + 0x2CABD + 5) = 0x00;
        VirtualProtect((LPVOID)(baseAddress + 0x2CABD), 6, oldProtect, &oldProtect);
    }
    else
    {
        DWORD oldProtect;
        VirtualProtect((LPVOID)(baseAddress + 0x1F9A6), 6, PAGE_EXECUTE_READWRITE, &oldProtect);
        *(std::uint8_t *)(baseAddress + 0x1F9A6 + 0) = 0x0F;
        *(std::uint8_t *)(baseAddress + 0x1F9A6 + 1) = 0x84;
        *(std::uint8_t *)(baseAddress + 0x1F9A6 + 2) = 0xD2;
        *(std::uint8_t *)(baseAddress + 0x1F9A6 + 3) = 0x00;
        *(std::uint8_t *)(baseAddress + 0x1F9A6 + 4) = 0x00;
        *(std::uint8_t *)(baseAddress + 0x1F9A6 + 5) = 0x00;
        VirtualProtect((LPVOID)(baseAddress + 0x1F9A6), 6, oldProtect, &oldProtect);
    }

    return true;
#elif __linux__
    // This is needed for custom menus, it allows new menu IDs to exist
    if (!*addresses::IsDedicated.ptr)
    {
        int oldProtect;
        VirtualProtect((void *)(baseAddress + 0x158FC8), 10, PAGE_EXECUTE_READWRITE, &oldProtect);
        NOP((std::uint8_t *)(baseAddress + 0x158FC8), 10);
        VirtualProtect((void *)(baseAddress + 0x158FC8), 10, oldProtect, &oldProtect);
    }

    return true;
#endif
}