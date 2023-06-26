#pragma once

#include <subhook.h>

extern subhook::Hook *drawMenuButtonSelectableHook;

int DrawMenuButtonSelectableHookFunc(const char *text, int *value, int selectedCmp);

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include <cstring>

#include "../Addresses.hpp"

subhook::Hook *drawMenuButtonSelectableHook;

int DrawMenuButtonSelectableHookFunc(const char *text, int *value, int selectedCmp)
{
    subhook::ScopedHookRemove scopedRemove(drawMenuButtonSelectableHook);

    if (*addresses::MenuTypeID != 3)
        return addresses::DrawMenuButtonSelectableFunc(text, value, selectedCmp); // we dont care if this is not options menu

#if _WIN32
    if ((std::uintptr_t)_ReturnAddress() - (std::uintptr_t)addresses::Base.ptr == 0xFEF27) // Push to talk key button
    {
        if (*addresses::MenuOptionsSectionID != 2)
            return 0;
    }
#elif __linux__
    if ((std::uintptr_t)__builtin_return_address(0) - (std::uintptr_t)addresses::Base.ptr == 0xAFA4E) // Push to talk key button
    {
        if (*addresses::MenuOptionsSectionID != 2)
            return 0;
    }
#endif

    return addresses::DrawMenuButtonSelectableFunc(text, value, selectedCmp);
}

#endif