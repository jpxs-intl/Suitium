#pragma once

#include <cstdint>

#include "Address.hpp"
#include "structs/Common.hpp"
#include "structs/Human.hpp"
#include "structs/ItemType.hpp"
#include "structs/Vehicle.hpp"
#include "structs/VehicleType.hpp"

namespace addresses
{
    extern DataAddress<void> Base;
    extern DataAddress<bool> IsDedicated;

    extern DataAddress<const int> GameVersionNumber;
    extern DataAddress<const std::uint8_t> GameVersionPatchNumber;

    extern DataAddress<structs::ItemType> ItemTypes;
    extern DataAddress<structs::VehicleType> VehicleTypes;
    extern DataAddress<structs::Human> Humans;
    extern DataAddress<structs::Vehicle> Vehicles;

    extern DataAddress<char> AuthName;
    extern DataAddress<int> AuthStatus;

    extern DataAddress<structs::CBoolean> SteamEnabled;
    extern DataAddress<structs::CBoolean> SteamTicketRetrieved;
    extern DataAddress<int> SteamTicketLength;
    extern DataAddress<char> SteamTicketBuffer;

    extern DataAddress<void *> SDLWindowPtr;

    using ConnectMasterServerFuncType = void ();
    extern FuncAddress<ConnectMasterServerFuncType> ConnectMasterServerFunc;

    using CreateVehicleFuncType = int (int typeID, structs::CVector3 *position, structs::CVector3 *velocity, structs::COrientation *orientation, int colorID);
    extern FuncAddress<CreateVehicleFuncType> CreateVehicleFunc;

#if _WIN32
    using CSDrawTextFuncType = std::int64_t (const char *format, float x, float y, float size, unsigned int flags, float red, float green, float blue, float alpha, ...);
#elif __linux__
    using CSDrawTextFuncType = std::int64_t (const char *format, int, int, int, float, float, float, float, float, float, float, void *);
#endif
    extern FuncAddress<CSDrawTextFuncType> CSDrawTextFunc;

    using MainMenuFuncType = void ();
    extern FuncAddress<MainMenuFuncType> MainMenuFunc;

    using PrintfFuncType = int (const char *format, ...);
    extern FuncAddress<PrintfFuncType> PrintfFunc;

    using SetupVehicleTypesFuncType = void ();
    extern FuncAddress<SetupVehicleTypesFuncType> SetupVehicleTypesFunc;

    bool Map(std::uintptr_t baseAddress);
}