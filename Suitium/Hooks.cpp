#include "Hooks.hpp"

#include <subhook.h>
#include <utility>
#include <vector>

#include "Addresses.hpp"

#define IMPLEMENT_HOOKS 1
#include "hooks/ConnectMasterServer.hpp"
#include "hooks/CSDrawText.hpp"
#include "hooks/MainMenu.hpp"

// FuncAddress, hook function and hook object
struct HookEntry {
  void** sourceFunction;
  void* hookedFunction;
  subhook::Hook** hook;
};

static std::vector<HookEntry> hookEntries =
{
    {(void **)&addresses::ConnectMasterServerFunc.ptr, (void *)&ConnectMasterServerHookFunc, &connectMasterServerHook},
    {(void **)&addresses::CSDrawTextFunc.ptr, (void *)&CSDrawTextHookFunc, &drawTextHook},
    {(void **)&addresses::MainMenuFunc.ptr, (void *)&MainMenuHookFunc, &mainMenuHook},
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