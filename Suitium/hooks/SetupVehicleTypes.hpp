#pragma once

#include <subhook.h>

extern subhook::Hook *setupVehicleTypesHook;

void SetupVehicleTypesHookFunc();

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include "../Addresses.hpp"
#include "../structs/VehicleType.hpp"

subhook::Hook *setupVehicleTypesHook;

void SetupVehicleTypesHookFunc()
{
    subhook::ScopedHookRemove scopedRemove(setupVehicleTypesHook);

    addresses::SetupVehicleTypesFunc();

    for (std::size_t vehicleTypeCount = 0; vehicleTypeCount < structs::VehicleType::VanillaCount; vehicleTypeCount++)
    {
        addresses::VehicleTypes[vehicleTypeCount].customData.index = vehicleTypeCount;
        addresses::VehicleTypes[vehicleTypeCount].customData.health = 100;
    }

    GetVehicleTypeManager()->Clear();
    for (std::size_t vehicleTypeCount = 0; vehicleTypeCount < structs::VehicleType::VanillaCount; vehicleTypeCount++)
    {
        std::stringstream stream;
        std::string vehicleTypeName = addresses::VehicleTypes[vehicleTypeCount].name;
        for (char c : vehicleTypeName)
        {
            if (c == ' ')
                stream << '_';
            else if (c == '-')
                continue;
            else if (c == '.')
                continue;
            else
                stream << (char)std::tolower(c);
        }

        // This is an empty type
        if (vehicleTypeCount == 14)
            stream << "unknown";

        GetVehicleTypeManager()->NewID("sub_rosa", stream.str());
        *addresses::VehicleTypes[vehicleTypeCount].customData.typeIDPtr = std::string("sub_rosa:") + stream.str();
    }
}

#endif