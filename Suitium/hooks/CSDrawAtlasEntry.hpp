#pragma once

#include <subhook.h>

extern subhook::Hook *csDrawAtlasEntryHook;

void CSDrawAtlasEntryHookFunc(int atlasID, int atlasEntryID, float x, float y, float width, float height, char unk1, float r, float g, float b, float a);

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include "../Addresses.hpp"

subhook::Hook *csDrawAtlasEntryHook;

void CSDrawAtlasEntryHookFunc(int textureID, int atlasEntryID, float x, float y, float width, float height, char unk1, float r, float g, float b, float a)
{
    subhook::ScopedHookRemove scopedRemove(csDrawAtlasEntryHook);

    addresses::CSDrawAtlasEntryFunc(textureID, atlasEntryID, x, y, width, height, unk1, r, g, b, a);
}

#endif