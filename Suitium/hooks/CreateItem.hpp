#pragma once

#include <subhook.h>

extern subhook::Hook *createItemHook;

int CreateItemHookFunc(int typeID, structs::CVector3 *position, structs::CVector3 *velocity, structs::COrientation *orientation);

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include "../Addresses.hpp"
#include "../structs/Common.hpp"
#include "../LuaManager.hpp"

subhook::Hook *createItemHook;

int CreateItemHookFunc(int typeID, structs::CVector3 *position, structs::CVector3 *velocity, structs::COrientation *orientation)
{
    subhook::ScopedHookRemove scopedRemove(createItemHook);

    structs::CInteger actualTypeID = typeID;
    structs::CVector3 actualPosition = (position != nullptr) ? *position : structs::CVector3();
    structs::CVector3 actualVelocity = (velocity != nullptr) ? *velocity : structs::CVector3();
    structs::COrientation actualOrientation = (orientation != nullptr) ? *orientation : structs::COrientation();

    GetMainLuaManager()->CallHooks("CreateItem", "pre", addresses::ItemTypes[actualTypeID.i], &actualPosition, &actualOrientation, &actualVelocity);

    int itemID = addresses::CreateItemFunc(actualTypeID.i, &actualPosition, &actualVelocity, &actualOrientation);
    if (itemID < 0)
        return -1;

    // Make sure these values are setup correctly
    addresses::Items[itemID].position = actualPosition;
    addresses::Items[itemID].velocity = actualVelocity;
    addresses::Items[itemID].orientation = actualOrientation;

    GetMainLuaManager()->CallHooks("CreateItem", "post", addresses::Items[itemID]);

    return itemID;
}

#endif