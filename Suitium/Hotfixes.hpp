#pragma once

#include <cstdint>

namespace hotfixes
{
    bool MakeForWin32(std::uintptr_t baseAddress);
    bool MakeForLinux(std::uintptr_t baseAddress);
}