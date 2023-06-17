#pragma once

void ResetGameHookFunc();

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include <subhook.h>

#include "../Addresses.hpp"
#include "../api/Logging.hpp"
#include "../LuaManager.hpp"

static subhook::Hook *resetGameHook;

void ResetGameHookFunc()
{
    subhook::ScopedHookRemove scopedRemove(resetGameHook);

    GetMainLuaManager()->CallHooks("ResetGame", "pre");

    api::GetSRLogger()->Log("Resetting game...");
    addresses::ResetGameFunc();

    GetMainLuaManager()->CallHooks("ResetGame", "post");
}

#endif