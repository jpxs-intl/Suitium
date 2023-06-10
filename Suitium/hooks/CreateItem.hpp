#pragma once

#include <subhook.h>

#include "../Addresses.hpp"
#include "../structs/Common.hpp"

int CreateItemHookFunc(int typeID, structs::CVector3 *position, structs::CVector3 *velocity, structs::COrientation *orientation);

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

static subhook::Hook *createItemHook;

int CreateItemHookFunc(int typeID, structs::CVector3 *position, structs::CVector3 *velocity, structs::COrientation *orientation)
{
    subhook::ScopedHookRemove scopedRemove(createItemHook);

    int itemID = addresses::CreateItemFunc(typeID, position, velocity, orientation);
    if (itemID < 0)
        return -1;

    return itemID;
}

#endif