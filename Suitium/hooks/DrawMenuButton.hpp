#pragma once

#include <subhook.h>

extern subhook::Hook *drawMenuButtonHook;

int DrawMenuButtonHookFunc(const char *text);

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include <cstring>

#include "../Addresses.hpp"

subhook::Hook *drawMenuButtonHook;

int DrawMenuButtonHookFunc(const char *text)
{
    subhook::ScopedHookRemove scopedRemove(drawMenuButtonHook);

    if (*addresses::MenuTypeID != 3)
        return addresses::DrawMenuButtonFunc(text); // we dont care if this is not options menu

    if (std::strcmp(text, "Copy System Info") == 0)
    {
        if (*addresses::MenuOptionsSectionID != 0)
            return 0;
        text = "Copy system info";
    }

    return addresses::DrawMenuButtonFunc(text);
}

#endif