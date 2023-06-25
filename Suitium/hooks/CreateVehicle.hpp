#pragma once

#include <subhook.h>

extern subhook::Hook *createVehicleHook;

int CreateVehicleHookFunc(int typeID, structs::CVector3 *position, structs::CVector3 *velocity, structs::COrientation *orientation, int colorID);

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include "../Addresses.hpp"
#include "../structs/Common.hpp"
#include "../LuaManager.hpp"

subhook::Hook *createVehicleHook;

int CreateVehicleHookFunc(int typeID, structs::CVector3 *position, structs::CVector3 *velocity, structs::COrientation *orientation, int colorID)
{
    subhook::ScopedHookRemove scopedRemove(createVehicleHook);

    structs::CInteger actualTypeID = typeID;
    structs::CInteger actualColorID = colorID;
    structs::CVector3 actualPosition;
    actualPosition = (position != nullptr) ? *position : structs::CVector3();
    structs::CVector3 actualVelocity;
    actualVelocity = (velocity != nullptr) ? *velocity : structs::CVector3();
    structs::COrientation actualOrientation;
    actualOrientation = (orientation != nullptr) ? *orientation : structs::COrientation();

    GetMainLuaManager()->CallHooks("CreateVehicle", "pre", addresses::VehicleTypes[actualTypeID.i], &actualColorID, &actualPosition, &actualOrientation, &actualVelocity);

    int vehicleID = addresses::CreateVehicleFunc(typeID, &actualPosition, &actualVelocity, &actualOrientation, colorID);
    if (vehicleID < 0)
        return -1;

    // Make sure these values are setup correctly
    addresses::Vehicles[vehicleID].position = actualPosition;
    addresses::Vehicles[vehicleID].velocity = actualVelocity;
    addresses::Vehicles[vehicleID].orientation = actualOrientation;
    addresses::Vehicles[vehicleID].health = addresses::VehicleTypes[typeID].customData.health;

    GetMainLuaManager()->CallHooks("CreateVehicle", "post", addresses::Vehicles[vehicleID]);

    return vehicleID;
}

#endif