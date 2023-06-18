#pragma once

#include <subhook.h>
#include <thread>

extern subhook::Hook *serverMainHook;

void ServerMainHookFunc();

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include <fmt/format.h>
#include <iostream>

#include "../Addon.hpp"
#include "../Addresses.hpp"
#include "../LuaManager.hpp"
#include "../structs/ItemType.hpp"
#include "../structs/VehicleType.hpp"
#include "../Version.hpp"

#if _WIN32
#include <Windows.h>
#endif

static bool inputRunning;
static std::thread inputThread;

subhook::Hook *serverMainHook;

void ServerMainHookFunc()
{
    subhook::ScopedHookRemove scopedRemove(serverMainHook);

    GetMainLuaManager()->Initialize();

    DiscoverAddons();
    for (auto it = GetAddons().begin(); it != GetAddons().end(); ++it)
        (*it)->Load(); // This loads in the ClientMain hook for the client
    for (auto it = GetAddons().begin(); it != GetAddons().end(); ++it)
    {
        if (!(*it)->IsLoaded())
            continue;
        if ((*it)->CheckDependencies())
            (*it)->PrepareLua(GetMainLuaManager(), true);
    }

    // We gotta setup the structs!
    for (std::size_t itemTypeCount = 0; itemTypeCount < structs::ItemType::VanillaCount; itemTypeCount++)
    {
        addresses::ItemTypes[itemTypeCount].customData.index = itemTypeCount;
        addresses::ItemTypes[itemTypeCount].customData.typeIDPtr = new std::string();
    }
    for (std::size_t vehicleTypeCount = 0; vehicleTypeCount < structs::VehicleType::VanillaCount; vehicleTypeCount++)
    {
        addresses::VehicleTypes[vehicleTypeCount].customData.index = vehicleTypeCount;
        addresses::VehicleTypes[vehicleTypeCount].customData.typeIDPtr = new std::string();
    }

    // TODO: We need to deinitialize Lua, for now it just crashes the server after closing the terminal

    inputRunning = true;
    inputThread = std::thread(
        [&]()
        {
            while (inputRunning)
            {
                std::string input;
                std::cin >> input;
            }
        }
    );

#if _WIN32
    if (*addresses::IsDedicated)
        SetConsoleTitle(fmt::format("Sub Rosa 0.{}{} (Dedicated): {}", SUITIUM_GAME_VERSION_NUMBER, SUITIUM_GAME_VERSION_PATCH_DEDICATED, addresses::ServerName.ptr).c_str());

    SetConsoleCtrlHandler(
        [](DWORD controlType)
        {
            inputRunning = false;
            inputThread.join();

            for (auto it = GetAddons().begin(); it != GetAddons().end(); ++it)
                (*it)->Unload();

            GetMainLuaManager()->Deinitialize();

            std::exit(0);

            return TRUE;
        },
        TRUE
    );
#endif

    addresses::ServerMainFunc();
}

#endif