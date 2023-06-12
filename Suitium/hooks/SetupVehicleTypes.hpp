#pragma once

void SetupVehicleTypesHookFunc();

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include <subhook.h>

#include "../Addresses.hpp"
#include "../structs/VehicleType.hpp"

static subhook::Hook *setupVehicleTypesHook;

void SetupVehicleTypesHookFunc()
{
    subhook::ScopedHookRemove scopedRemove(setupVehicleTypesHook);

    addresses::SetupVehicleTypesFunc();

    for (int vehicleTypeID = 0; vehicleTypeID < structs::VehicleType::VanillaCount; vehicleTypeID++)
        addresses::VehicleTypes[vehicleTypeID].customData.health = 100; // lets just make it the default for now
}

#endif