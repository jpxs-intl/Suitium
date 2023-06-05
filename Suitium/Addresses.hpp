#pragma once

#include <cstdint>

#include "Address.hpp"

namespace addresses
{
    extern DataAddress<int> GameVersionNumber;
    extern DataAddress<std::uint8_t> GameVersionPatchNumber;

    extern DataAddress<char> AuthName;
    extern DataAddress<int> AuthStatus;

    extern DataAddress<int> SteamEnabled;
    extern DataAddress<int> SteamTicketLength;
    extern DataAddress<char> SteamTicketBuffer;
    extern DataAddress<int> SteamTicketRetrieved;

    using ConnectMasterServerFuncType = void ();
    extern FuncAddress<ConnectMasterServerFuncType> ConnectMasterServerFunc;

#if _WIN32
    using DrawTextFuncType = std::int64_t (const char *format, float x, float y, float size, unsigned int flags, float red, float green, float blue, float alpha, ...);
#elif __linux__
    using DrawTextFuncType = std::int64_t (const char *format, int, int, int, float, float, float, float, float, float, float, void *);
#endif
    extern FuncAddress<DrawTextFuncType> DrawTextFunc;
}

bool MapAddressesForWin32(std::uintptr_t baseAddress);
bool MapAddressesForLinux(std::uintptr_t baseAddress);