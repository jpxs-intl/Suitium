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
using HookEntry = std::pair<void **, std::pair<void *, subhook::Hook **>>;

static std::vector<HookEntry> hookEntries = 
{
    std::make_pair((void **)&addresses::ConnectMasterServerFunc.ptr, std::make_pair((void *)&ConnectMasterServerHookFunc, &connectMasterServerHook)),
    std::make_pair((void **)&addresses::CSDrawTextFunc.ptr, std::make_pair((void *)&CSDrawTextHookFunc, &drawTextHook)),
    std::make_pair((void **)&addresses::MainMenuFunc.ptr, std::make_pair((void *)&MainMenuHookFunc, &mainMenuHook))
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