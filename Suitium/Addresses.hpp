#pragma once

#include <cstdint>

#include "Address.hpp"

#include "structs/ItemType.hpp"
#include "structs/VehicleType.hpp"

namespace addresses
{
    extern DataAddress<int> GameVersionNumber;
    extern DataAddress<std::uint8_t> GameVersionPatchNumber;

    extern DataAddress<ItemType> ItemTypes;
    extern DataAddress<VehicleType> VehicleTypes;

    extern DataAddress<char> AuthName;
    extern DataAddress<int> AuthStatus;

    extern DataAddress<int> SteamEnabled;
    extern DataAddress<int> SteamTicketLength;
    extern DataAddress<char> SteamTicketBuffer;
    extern DataAddress<int> SteamTicketRetrieved;

    using ConnectMasterServerFuncType = void ();
    extern FuncAddress<ConnectMasterServerFuncType> ConnectMasterServerFunc;

#if _WIN32
    using CSDrawTextFuncType = std::int64_t (const char *format, float x, float y, float size, unsigned int flags, float red, float green, float blue, float alpha, ...);
#elif __linux__
    using CSDrawTextFuncType = std::int64_t (const char *format, int, int, int, float, float, float, float, float, float, float, void *);
#endif
    extern FuncAddress<CSDrawTextFuncType> CSDrawTextFunc;
}

bool MapAddressesForWin32(std::uintptr_t baseAddress);
bool MapAddressesForLinux(std::uintptr_t baseAddress);