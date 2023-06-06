#pragma once

#include <cstdint>

bool MakeHotfixesForWin32(std::uintptr_t baseAddress);
bool MakeHotfixesForLinux(std::uintptr_t baseAddress);