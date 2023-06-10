#pragma once

#include <subhook.h>

#include "../Addresses.hpp"

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

    return addresses::ConnectMasterServerFunc(); // This might be used in the future
}

#endif