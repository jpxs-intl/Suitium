#include <subhook.h>

#include "../AddressInterface.hpp"
#include "../MasterServer.hpp"

#if _WIN32
using ConnectMasterServerFunc = void (*)();
#elif __linux__
using ConnectMasterServerFunc = void (*)();
#endif

#if _WIN32
void ConnectMasterServerHookFunc();
#elif __linux__
void ConnectMasterServerHookFunc();
#endif

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

static subhook::Hook *connectMasterServerHook;

void ConnectMasterServerHookFunc()
{
    subhook::ScopedHookRemove scopedRemove(connectMasterServerHook);

    const AddressInterface::AddressMap &addressMap = GetAddressInterface()->GetAddressMap();
    ConnectMasterServerFunc originalFunc = (ConnectMasterServerFunc)(GetBaseAddress() + addressMap.at(AddressType::ConnectMasterServerFunc));

    MasterServer::GetSingleton()->Connect("ms.jpxs.io"); // TODO: customization?
}

#endif