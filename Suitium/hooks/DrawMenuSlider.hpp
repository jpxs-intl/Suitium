#pragma once

#include <subhook.h>

extern subhook::Hook *drawMenuSliderHook;

int DrawMenuSliderHookFunc(const char *text, int *value, int minValue, int maxValue, int unk);

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include <cstring>

#include "../Addresses.hpp"

subhook::Hook *drawMenuSliderHook;

int DrawMenuSliderHookFunc(const char *text, int *value, int minValue, int maxValue, int unk)
{
    subhook::ScopedHookRemove scopedRemove(drawMenuSliderHook);

    if (*addresses::MenuTypeID != 3)
        return addresses::DrawMenuSliderFunc(text, value, minValue, maxValue, unk); // we dont care if this is not options menu

    if (std::strcmp(text, "Sound Volume") == 0)
    {
        if (*addresses::MenuOptionsSectionID != 2)
            return 0;
        *addresses::NextMenuButtonPositionX = 4.0f;
        *addresses::NextMenuButtonPositionY = 120.0f;
        text = "Output volume";
    }

    return addresses::DrawMenuSliderFunc(text, value, minValue, maxValue, unk);
}

#endif