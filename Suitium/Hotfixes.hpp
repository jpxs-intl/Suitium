#pragma once

#include <cstdint>

#define NOP(address, size) std::memset(address, 0x90, size);

#if __linux__
#define PAGE_READONLY           0x02    
#define PAGE_READWRITE          0x04
#define PAGE_EXECUTE            0x10    
#define PAGE_EXECUTE_READ       0x20    
#define PAGE_EXECUTE_READWRITE  0x40
int VirtualProtect(void *address, std::size_t size, int protection, int *oldProtection);
#endif

namespace hotfixes
{
    bool Make(std::uintptr_t baseAddress);
}