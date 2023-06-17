#pragma once

void LoadServerHookFunc();

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include <fmt/format.h>
#include <subhook.h>

#include "../Addon.hpp"
#include "../Addresses.hpp"
#include "../LuaManager.hpp"
#include "../Version.hpp"

#if _WIN32
#include <Windows.h>
#endif

static subhook::Hook *loadServerHook;

void LoadServerHookFunc()
{
    subhook::ScopedHookRemove scopedRemove(loadServerHook);

    addresses::LoadServerFunc();

    GetMainLuaManager()->Initialize();

    DiscoverAddons();
    for (auto it = GetAddons().begin(); it != GetAddons().end(); ++it)
        (*it)->Load(); // This loads in the ClientMain hook for the client
    for (auto it = GetAddons().begin(); it != GetAddons().end(); ++it)
        (*it)->CheckDependencies();
    for (auto it = GetAddons().begin(); it != GetAddons().end(); ++it)
        (*it)->PrepareLua(GetMainLuaManager());

#if _WIN32
    if (*addresses::IsDedicated)
        SetConsoleTitle(fmt::format("Sub Rosa 0.{}{} (Dedicated): {}", SUITIUM_GAME_VERSION_NUMBER, SUITIUM_GAME_VERSION_PATCH_DEDICATED, addresses::ServerName.ptr).c_str());
#endif
}

#endif