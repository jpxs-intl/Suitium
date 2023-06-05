#include "Hooks.hpp"

#include <subhook.h>
#include <utility>
#include <vector>

#include "Address.hpp"
#include "Addresses.hpp"

#define IMPLEMENT_HOOKS 1
#include "hooks/ConnectMasterServer.hpp"
#include "hooks/DrawText.hpp"

// FuncAddress, hook function and hook object
using HookEntry = std::pair<void **, std::pair<void *, subhook::Hook **>>;

static std::vector<HookEntry> hookEntries = 
{
    std::make_pair((void **)&addresses::ConnectMasterServerFunc.ptr, std::make_pair((void *)&ConnectMasterServerHookFunc, &connectMasterServerHook)),
    std::make_pair((void **)&addresses::CSDrawTextFunc.ptr, std::make_pair((void *)&CSDrawTextHookFunc, &drawTextHook))
};

void PrepareHooks()
{
    for (auto it = hookEntries.begin(); it != hookEntries.end(); ++it)
    {
        subhook::Hook *hook = new subhook::Hook(*(*it).first, (*it).second.first, subhook::HookFlag64BitOffset);
        hook->Install();

        *(*it).second.second = hook;
    }
}