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

    structs::CVector3 actualPosition;
    actualPosition = (position != nullptr) ? *position : glm::vec3();
    structs::CVector3 actualVelocity;
    actualVelocity = (velocity != nullptr) ? *velocity : glm::vec3();
    structs::COrientation actualOrientation;
    actualOrientation = (orientation != nullptr) ? *orientation : glm::mat3(1.0f);

    int itemID = addresses::CreateItemFunc(typeID, &actualPosition, &actualVelocity, &actualOrientation);
    if (itemID < 0)
        return -1;

    // Make sure these values are setup correctly
    addresses::Items[itemID].position = actualPosition;
    addresses::Items[itemID].velocity = actualVelocity;
    addresses::Items[itemID].orientation = actualOrientation;

    return itemID;
}

#endif