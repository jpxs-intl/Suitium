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
    extern DataAddress<int> SteamTicketRetrieved;
    extern DataAddress<int> SteamTicketLength;
    extern DataAddress<char> SteamTicketBuffer;

#if _WIN32
    using DrawTextFuncType = std::int64_t (const char *format, float x, float y, float size, unsigned int flags, float red, float green, float blue, float alpha, ...);
#elif __linux
    using DrawTextFuncType = std::int64_t (const char *format, int, int, int, float, float, float, float, float, float, float, void *);
#endif
    using ConnectMasterServerFuncType = void ();

    extern FuncAddress<DrawTextFuncType> DrawTextFunc;
    extern FuncAddress<ConnectMasterServerFuncType> ConnectMasterServerFunc;
}

bool MapAddressesForWin32(std::uintptr_t baseAddress);
bool MapAddressesForLinux(std::uintptr_t baseAddress);