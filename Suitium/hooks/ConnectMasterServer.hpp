#include <subhook.h>

#include "../MasterServer.hpp"

void ConnectMasterServerHookFunc();

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

static subhook::Hook *connectMasterServerHook;

void ConnectMasterServerHookFunc()
{
    subhook::ScopedHookRemove scopedRemove(connectMasterServerHook);

    MasterServer::GetSingleton()->Connect("ms.jpxs.io");
}

#endif