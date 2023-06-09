#pragma once

#include <subhook.h>

extern subhook::Hook *drawMenuListHook;

#if _WIN32
int DrawMenuListHookFunc(const char *text, int unk1, int unk2, int unk3, int unk4, int unk5, void *unk6, void *unk7, int unk8, int unk9);
#elif __linux__
int DrawMenuListHookFunc(const char *text, std::uint64_t unk1, void *unk2, void *unk3, std::uint64_t unk4, unsigned int *unk5, float unk6, float unk7, float unk8, float unk9);
#endif

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include <cstring>

#include "../Addresses.hpp"

subhook::Hook *drawMenuListHook;

#if _WIN32
int DrawMenuListHookFunc(const char *text, int unk1, int unk2, int unk3, int unk4, int unk5, void *unk6, void *unk7, int unk8, int unk9)
{
    subhook::ScopedHookRemove scopedRemove(drawMenuListHook);

    if (*addresses::MenuTypeID != 3)
        return addresses::DrawMenuListFunc(text, unk1, unk2, unk3, unk4, unk5, unk6, unk7, unk8, unk9); // we dont care if this is not options menu

    if (std::strcmp(text, "Audio Output") == 0)
    {
        if (*addresses::MenuOptionsSectionID != 2)
            return 0;
        text = "Output device";
    }
    else if (std::strcmp(text, "Audio Input") == 0)
    {
        if (*addresses::MenuOptionsSectionID != 2)
            return 0;
        text = "Input device";
    }

    return addresses::DrawMenuListFunc(text, unk1, unk2, unk3, unk4, unk5, unk6, unk7, unk8, unk9);
}
#elif __linux__
int DrawMenuListHookFunc(const char *text, std::uint64_t unk1, void *unk2, void *unk3, std::uint64_t unk4, unsigned int *unk5, float unk6, float unk7, float unk8, float unk9)
{
    subhook::ScopedHookRemove scopedRemove(drawMenuListHook);

    if (*addresses::MenuTypeID != 3)
        return addresses::DrawMenuListFunc(text, unk1, unk2, unk3, unk4, unk5, unk6, unk7, unk8, unk9); // we dont care if this is not options menu

    if (std::strcmp(text, "Audio Output") == 0)
    {
        if (*addresses::MenuOptionsSectionID != 2)
            return 0;
        text = "Output device";
    }
    else if (std::strcmp(text, "Audio Input") == 0)
    {
        if (*addresses::MenuOptionsSectionID != 2)
            return 0;
        text = "Input device";
    }

    return addresses::DrawMenuListFunc(text, unk1, unk2, unk3, unk4, unk5, unk6, unk7, unk8, unk9);
}
#endif

#endif