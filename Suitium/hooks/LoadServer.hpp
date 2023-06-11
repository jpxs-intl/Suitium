#pragma once

#include <subhook.h>

#include "../Addon.hpp"
#include "../Addresses.hpp"
#include "../Version.hpp"

#if _WIN32
#include <Windows.h>
#endif

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

#if _WIN32
    if (*addresses::IsDedicated)
        SetConsoleTitle(fmt::format("Sub Rosa 0.{}{} (Dedicated): {}", SUITIUM_GAME_VERSION_NUMBER, SUITIUM_GAME_VERSION_PATCH_DEDICATED, addresses::ServerName.ptr).c_str());
#endif
}

#endif