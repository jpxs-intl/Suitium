#pragma once

#include <subhook.h>

#include "../Addresses.hpp"
#include "../api/Logging.hpp"

void ResetGameHookFunc();

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

static subhook::Hook *resetGameHook;

void ResetGameHookFunc()
{
    subhook::ScopedHookRemove scopedRemove(resetGameHook);

    api::GetSRLogger()->Log("Resetting game...");

    addresses::ResetGameFunc();
}

#endif