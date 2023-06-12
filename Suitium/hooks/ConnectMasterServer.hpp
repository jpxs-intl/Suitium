#pragma once

void ConnectMasterServerHookFunc();

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include <subhook.h>

#include "../Addresses.hpp"

static subhook::Hook *connectMasterServerHook;

void ConnectMasterServerHookFunc()
{
    subhook::ScopedHookRemove scopedRemove(connectMasterServerHook);

    addresses::ConnectMasterServerFunc(); // This might be used in the future
}

#endif