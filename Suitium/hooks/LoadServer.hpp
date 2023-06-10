#pragma once

#include <subhook.h>

#include "../Addon.hpp"
#include "../Addresses.hpp"

void LoadServerHookFunc();

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

static subhook::Hook *loadServerHook;

void LoadServerHookFunc()
{
    subhook::ScopedHookRemove scopedRemove(loadServerHook);

    if (*addresses::IsDedicated)
    {
        DiscoverAddons();
        for (auto it = GetAddons().begin(); it != GetAddons().end(); ++it)
            (*it)->Load(); // This loads in the MainMenu hook for the client
        for (auto it = GetAddons().begin(); it != GetAddons().end(); ++it)
            (*it)->CheckDependencies();
    }

    addresses::LoadServerFunc();
}

#endif