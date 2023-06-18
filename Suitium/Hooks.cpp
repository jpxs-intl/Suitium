#include "Hooks.hpp"

#include <subhook.h>
#include <utility>
#include <vector>

#include "Addresses.hpp"

#define IMPLEMENT_HOOKS 1
#include "hooks/CSDrawText.hpp"
#include "hooks/ClientMain.hpp"
#include "hooks/DrawMenu.hpp"
#include "hooks/ConnectMasterServer.hpp"
#include "hooks/CreateItem.hpp"
#include "hooks/CreateVehicle.hpp"
#include "hooks/Printf.hpp"
#include "hooks/ResetGame.hpp"
#include "hooks/ServerMain.hpp"
#include "hooks/SetupItemTypes.hpp"
#include "hooks/SetupVehicleTypes.hpp"

// FuncAddress, hook function and hook object
struct HookEntry 
{
    void** sourceFunction;
    void* hookedFunction;
    subhook::Hook** hook;
};

void InstallHooks()
{
    static std::vector<HookEntry> hookEntries =
    {
        {(void **)&addresses::ConnectMasterServerFunc.ptr, (void *)&ConnectMasterServerHookFunc, &connectMasterServerHook},
        {(void **)&addresses::CreateItemFunc.ptr, (void *)&CreateItemHookFunc, &createItemHook},
        {(void **)&addresses::CreateVehicleFunc.ptr, (void *)&CreateVehicleHookFunc, &createVehicleHook},
        {(void **)&addresses::CSDrawTextFunc.ptr, (void *)&CSDrawTextHookFunc, &drawTextHook},
        {(void **)&addresses::ClientMainFunc.ptr, (void *)&ClientMainHookFunc, &clientMainHook},
        {(void **)&addresses::ResetGameFunc.ptr, (void *)&ResetGameHookFunc, &resetGameHook},
        {(void **)&addresses::DrawMenuFunc.ptr, (void *)&DrawMenuHookFunc, &drawMenuHook},
        {(void **)&addresses::PrintfFunc.ptr, (void *)&PrintfHookFunc, &printfHook},
        {(void **)&addresses::ServerMainFunc.ptr, (void *)&ServerMainHookFunc, &serverMainHook},
        {(void **)&addresses::SetupItemTypesFunc.ptr, (void *)&SetupItemTypesHookFunc, &setupItemTypesHook},
        {(void **)&addresses::SetupVehicleTypesFunc.ptr, (void *)&SetupVehicleTypesHookFunc, &setupVehicleTypesHook}
    };

    for (HookEntry entry : hookEntries) 
    {
        if (!*entry.sourceFunction) // Some versions might not contain one or two hooks
          continue ;

        subhook::Hook *hook = new subhook::Hook(*entry.sourceFunction, entry.hookedFunction, subhook::HookFlag64BitOffset);
        hook->Install();

        *entry.hook = hook;
    }
}