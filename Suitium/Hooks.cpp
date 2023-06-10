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
#include "hooks/MainMenu.hpp"
#include "hooks/Printf.hpp"
#include "hooks/SetupVehicleTypes.hpp"

// FuncAddress, hook function and hook object
struct HookEntry {
  void** sourceFunction;
  void* hookedFunction;
  subhook::Hook** hook;
};

static std::vector<HookEntry> hookEntries =
{
    {(void **)&addresses::ConnectMasterServerFunc.ptr, (void *)&ConnectMasterServerHookFunc, &connectMasterServerHook},
    {(void **)&addresses::CreateItemFunc.ptr, (void *)&CreateItemHookFunc, &createItemHook},
    {(void **)&addresses::CreateVehicleFunc.ptr, (void *)&CreateVehicleHookFunc, &createVehicleHook},
    {(void **)&addresses::CSDrawTextFunc.ptr, (void *)&CSDrawTextHookFunc, &drawTextHook},
    {(void **)&addresses::MainMenuFunc.ptr, (void *)&MainMenuHookFunc, &mainMenuHook},
    {(void **)&addresses::PrintfFunc.ptr, (void *)&PrintfHookFunc, &printfHook},
    {(void **)&addresses::SetupVehicleTypesFunc.ptr, (void *)&SetupVehicleTypesHookFunc, &setupVehicleTypesHook},
};

void InstallHooks()
{
    for (auto entry : hookEntries) {
        if (!*entry.sourceFunction) // Some versions might not contain one or two hooks
          continue ;

        subhook::Hook *hook = new subhook::Hook(*entry.sourceFunction, entry.hookedFunction, subhook::HookFlag64BitOffset);
        hook->Install();

        *entry.hook = hook;
    }
}