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
//#include "../api/Logging.hpp"
#include "../structs/ItemType.hpp"
#include "../TypeManager.hpp"

subhook::Hook *setupItemTypesHook;

void SetupItemTypesHookFunc()
{
    subhook::ScopedHookRemove scopedRemove(setupItemTypesHook);

    addresses::SetupItemTypesFunc();

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

        // There are two cash types for whatever reason, let's add a postfix for both
        if (itemTypeCount == 17)
            stream << '1';
        if (itemTypeCount == 18)
            stream << '2';

        GetItemTypeManager()->NewID("sub_rosa", stream.str());
        //api::GetSuitiumLogger()->Log("{} ({}) registered as \"sub_rosa:{}\"", addresses::ItemTypes[itemTypeCount].name, itemTypeCount, stream.str());
    }
}

#endif