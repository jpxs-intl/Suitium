#include "Hooks.hpp"

#include <array>
#include <cstdint>
#include <memory>
#include <subhook.h>
#include <utility>

#include "AddressInterface.hpp"

#define IMPLEMENT_HOOKS 1
#include "hooks/ConnectMasterServer.hpp"
#include "hooks/DrawText.hpp"

using HookEntry = std::pair<std::intptr_t, subhook::Hook **>;

static const std::unordered_map<AddressType, HookEntry> hookEntryMap = {
    std::make_pair(AddressType::DrawTextFunc, std::make_pair((std::intptr_t)DrawTextHookFunc, &drawTextHook)),
    std::make_pair(AddressType::ConnectMasterServerFunc, std::make_pair((std::intptr_t)ConnectMasterServerHookFunc, &connectMasterServerHook))
};

void PrepareHooks()
{
    const AddressInterface::AddressMap &addressMap = GetAddressInterface()->GetAddressMap();
    for (std::size_t addressCount = 0; addressCount < (std::size_t)AddressType::_Count; addressCount++)
    {
        if (addressCount < (std::size_t)AddressType::DrawTextFunc || addressCount >= (std::size_t)AddressType::_Count)
            continue;

        if (addressMap.count((AddressType)addressCount) > 0 && hookEntryMap.count((AddressType)addressCount) > 0)
        {
            HookEntry hookEntry = hookEntryMap.at((AddressType)addressCount);
            *hookEntry.second = new subhook::Hook((void *)(GetBaseAddress() + addressMap.at((AddressType)addressCount)), (void *)hookEntry.first, subhook::HookFlag64BitOffset); // TODO: we really want to use new?
            
            (*hookEntry.second)->Install();
        }
    }
}