#pragma once

#include <subhook.h>

extern subhook::Hook *csDrawAtlasEntryHook;

void CSDrawAtlasEntryHookFunc(int atlasID, int atlasEntryID, float x, float y, float width, float height, char unk1, int unk2, int unk3, int unk4, int unk5);

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include "../Addresses.hpp"

subhook::Hook *csDrawAtlasEntryHook;

void CSDrawAtlasEntryHookFunc(int atlasID, int atlasEntryID, float x, float y, float width, float height, char unk1, int unk2, int unk3, int unk4, int unk5)
{
    subhook::ScopedHookRemove scopedRemove(csDrawAtlasEntryHook);

    addresses::CSDrawAtlasEntryFunc(atlasID, atlasEntryID, x, y, width, height, unk1, unk2, unk3, unk4, unk5);
}

#endif