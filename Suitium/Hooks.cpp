#include "Hooks.hpp"

#include <subhook.h>
#include <utility>
#include <vector>

#include "Addresses.hpp"

#define IMPLEMENT_HOOKS 1
#include "hooks/ConnectMasterServer.hpp"
#include "hooks/CreateItem.hpp"
#include "hooks/CreateVehicle.hpp"
#include "hooks/CSDrawText.hpp"
#include "hooks/LoadServer.hpp"
#include "hooks/MainMenu.hpp"
#include "hooks/Printf.hpp"
#include "hooks/SetupVehicleTypes.hpp"

// FuncAddress, hook function and hook object
using HookEntry = std::pair<void **, std::pair<void *, subhook::Hook **>>;

static std::vector<HookEntry> hookEntries = 
{
    std::make_pair((void **)&addresses::ConnectMasterServerFunc.ptr, std::make_pair((void *)&ConnectMasterServerHookFunc, &connectMasterServerHook)),
    std::make_pair((void **)&addresses::CreateItemFunc.ptr, std::make_pair((void *)&CreateItemHookFunc, &createItemHook)),
    std::make_pair((void **)&addresses::CreateVehicleFunc.ptr, std::make_pair((void *)&CreateVehicleHookFunc, &createVehicleHook)),
    std::make_pair((void **)&addresses::CSDrawTextFunc.ptr, std::make_pair((void *)&CSDrawTextHookFunc, &drawTextHook)),
    std::make_pair((void **)&addresses::LoadServerFunc.ptr, std::make_pair((void *)&LoadServerHookFunc, &loadServerHook)),
    std::make_pair((void **)&addresses::MainMenuFunc.ptr, std::make_pair((void *)&MainMenuHookFunc, &mainMenuHook)),
    std::make_pair((void **)&addresses::PrintfFunc.ptr, std::make_pair((void *)&PrintfHookFunc, &printfHook)),
    std::make_pair((void **)&addresses::SetupVehicleTypesFunc.ptr, std::make_pair((void *)&SetupVehicleTypesHookFunc, &setupVehicleTypesHook))
};

void InstallHooks()
{
    for (auto it = hookEntries.begin(); it != hookEntries.end(); ++it)
    {
        if (!*(*it).first) // Some versions might not contain one or two hooks
            continue;

        subhook::Hook *hook = new subhook::Hook(*(*it).first, (*it).second.first, subhook::HookFlag64BitOffset);
        hook->Install();

        *(*it).second.second = hook;
    }
}