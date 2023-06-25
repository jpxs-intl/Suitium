#pragma once

#include <subhook.h>

extern subhook::Hook *drawMenuTextBoxHook;

int DrawMenuTextBoxHookFunc(const char *text, int *value, int unk1, int unk2);

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include <cstring>

#include "../Addresses.hpp"

subhook::Hook *drawMenuTextBoxHook;

int DrawMenuTextBoxHookFunc(const char *text, int *value, int unk1, int unk2)
{
    subhook::ScopedHookRemove scopedRemove(drawMenuTextBoxHook);

    if (*addresses::MenuTypeID != 3)
        return addresses::DrawMenuTextBoxFunc(text, value, unk1, unk2); // we dont care if this is not options menu

    if (std::strcmp(text, "Port") == 0)
    {
        if (*addresses::MenuOptionsSectionID != 0)
            return 0;
        *addresses::NextMenuButtonPositionX = 4.0f;
        *addresses::NextMenuButtonPositionY = 120.0f;
    }
    else if (std::strcmp(text, "Bandwidth") == 0)
    {
        if (*addresses::MenuOptionsSectionID != 0)
            return 0;
    }
    
    return addresses::DrawMenuTextBoxFunc(text, value, unk1, unk2);
}

#endif