#include <subhook.h>

#include "../Addresses.hpp"
#include "../structs/Common.hpp"

int CreateVehicleHookFunc(int typeID, structs::CVector3 *position, structs::CVector3 *velocity, structs::COrientation *orientation, int colorID);

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

static subhook::Hook *createVehicleHook;

int CreateVehicleHookFunc(int typeID, structs::CVector3 *position, structs::CVector3 *velocity, structs::COrientation *orientation, int colorID)
{
    subhook::ScopedHookRemove scopedRemove(createVehicleHook);

    int vehicleID = addresses::CreateVehicleFunc(typeID, position, velocity, orientation, colorID);
    return vehicleID;
}

#endif