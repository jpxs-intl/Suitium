#pragma once

#include <subhook.h>

extern subhook::Hook *drawMenuToggleHook;

int DrawMenuToggleHookFunc(const char *text, int *value);

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include <cstring>

#include "../Addresses.hpp"

subhook::Hook *drawMenuToggleHook;

int DrawMenuToggleHookFunc(const char *text, int *value)
{
    subhook::ScopedHookRemove scopedRemove(drawMenuToggleHook);

    if (*addresses::MenuTypeID != 3)
        return addresses::DrawMenuToggleFunc(text, value); // we dont care if this is not options menu

    if (std::strcmp(text, "Fullscreen") == 0)
    {
        if (*addresses::MenuOptionsSectionID != 1)
            return 0;
        *addresses::NextMenuButtonPositionX = 4.0f;
        *addresses::NextMenuButtonPositionY = 120.0f;
        text = "Enable fullscreen";
    }
    if (std::strcmp(text, "V-Sync") == 0)
    {
        if (*addresses::MenuOptionsSectionID != 1)
            return 0;
        text = "Enable v-sync";
    }
    else if (std::strcmp(text, "Frame Smoothing") == 0)
    {
        *value = 0;
        return 0; // Frame smoothing is broken right now, let's disable it
    }
    else if (std::strcmp(text, "Sprite Models") == 0)
    {
        if (*addresses::MenuOptionsSectionID != 1)
            return 0;
        text = "Enable debris particles";
    }
    else if (std::strcmp(text, "Invert Mouse") == 0)
    {
        if (*addresses::MenuOptionsSectionID != 3)
            return 0;
        *addresses::NextMenuButtonPositionX = 4.0f;
        *addresses::NextMenuButtonPositionY = 120.0f;
        text = "Invert mouse";
    }
    else if (std::strcmp(text, "Use advanced inventory") == 0)
    {
        if (*addresses::MenuOptionsSectionID != 3)
            return 0;
    }
    else if (std::strcmp(text, "Use Push to Talk") == 0)
    {
        if (*addresses::MenuOptionsSectionID != 2)
            return 0;
        text = "Use push to talk";
    }

    return addresses::DrawMenuToggleFunc(text, value);
}

#endif