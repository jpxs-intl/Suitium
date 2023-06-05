#pragma once

#include <cstdint>

#include "Address.hpp"
#include "structs/Common.hpp"
#include "structs/ItemType.hpp"
#include "structs/VehicleType.hpp"

namespace addresses
{
    extern DataAddress<bool> IsDedicated;

    extern DataAddress<int> GameVersionNumber;
    extern DataAddress<std::uint8_t> GameVersionPatchNumber;

    extern DataAddress<structs::ItemType> ItemTypes;
    extern DataAddress<structs::VehicleType> VehicleTypes;

    extern DataAddress<char> AuthName;
    extern DataAddress<int> AuthStatus;

    extern DataAddress<structs::CBoolean> SteamEnabled;
    extern DataAddress<structs::CBoolean> SteamTicketRetrieved;
    extern DataAddress<int> SteamTicketLength;
    extern DataAddress<char> SteamTicketBuffer;

    extern DataAddress<void *> SDLWindowPtr;

    using ConnectMasterServerFuncType = void ();
    extern FuncAddress<ConnectMasterServerFuncType> ConnectMasterServerFunc;

    using MainMenuFuncType = void ();
    extern FuncAddress<MainMenuFuncType> MainMenuFunc;

#if _WIN32
    using CSDrawTextFuncType = std::int64_t (const char *format, float x, float y, float size, unsigned int flags, float red, float green, float blue, float alpha, ...);
#elif __linux__
    using CSDrawTextFuncType = std::int64_t (const char *format, int, int, int, float, float, float, float, float, float, float, void *);
#endif
    extern FuncAddress<CSDrawTextFuncType> CSDrawTextFunc;
}

bool MapAddressesForWin32(std::uintptr_t baseAddress);
bool MapAddressesForLinux(std::uintptr_t baseAddress);