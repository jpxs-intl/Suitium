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

subhook::Hook *createVehicleHook;

int CreateVehicleHookFunc(int typeID, structs::CVector3 *position, structs::CVector3 *velocity, structs::COrientation *orientation, int colorID)
{
    subhook::ScopedHookRemove scopedRemove(createVehicleHook);

    structs::CVector3 actualPosition;
    actualPosition = (position != nullptr) ? *position : glm::vec3();
    structs::CVector3 actualVelocity;
    actualVelocity = (velocity != nullptr) ? *velocity : glm::vec3();
    structs::COrientation actualOrientation;
    actualOrientation = (orientation != nullptr) ? *orientation : glm::mat3(1.0f);

    int vehicleID = addresses::CreateVehicleFunc(typeID, &actualPosition, &actualVelocity, &actualOrientation, colorID);
    if (vehicleID < 0)
        return -1;

    // Make sure these values are setup correctly
    addresses::Vehicles[vehicleID].position = actualPosition;
    addresses::Vehicles[vehicleID].velocity = actualVelocity;
    addresses::Vehicles[vehicleID].orientation = actualOrientation;
    addresses::Vehicles[vehicleID].health = addresses::VehicleTypes[typeID].customData.health;

    return vehicleID;
}

#endif