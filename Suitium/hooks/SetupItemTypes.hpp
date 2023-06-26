#pragma once

#include <cctype>
#include <subhook.h>

extern subhook::Hook *setupItemTypesHook;

void SetupItemTypesHookFunc();

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include "../Addresses.hpp"
#include "../LuaManager.hpp"
#include "../structs/ItemType.hpp"
#include "../TypeManager.hpp"

subhook::Hook *setupItemTypesHook;

void SetupItemTypesHookFunc()
{
    subhook::ScopedHookRemove scopedRemove(setupItemTypesHook);

    GetMainLuaManager()->CallHooks("SetupItemTypes", "pre");

    addresses::SetupItemTypesFunc();

    for (std::size_t itemTypeCount = 0; itemTypeCount < structs::ItemType::VanillaCount; itemTypeCount++)
        addresses::ItemTypes[itemTypeCount].customData.index = itemTypeCount;
    
    GetItemTypeManager()->Clear();
    for (std::size_t itemTypeCount = 0; itemTypeCount < structs::ItemType::VanillaCount; itemTypeCount++)
    {
        std::stringstream stream;
        std::string itemTypeName = addresses::ItemTypes[itemTypeCount].name;
        for (char c : itemTypeName)
        {
            if (c == ' ')
                stream << '_';
            else if (c == '-')
                continue;
            else if (c == '.')
                continue;
            else
                stream << (char )std::tolower(c);
        }

        // There are two different cash types for round mode and world mode
        if (itemTypeCount == 17)
            stream << '1';
        if (itemTypeCount == 18)
            stream << '2';

        GetItemTypeManager()->NewID("sub_rosa", stream.str());
        *addresses::ItemTypes[itemTypeCount].customData.typeIDPtr = std::string("sub_rosa:") + stream.str();
    }

    GetMainLuaManager()->CallHooks("SetupItemTypes", "post");
}

#endif