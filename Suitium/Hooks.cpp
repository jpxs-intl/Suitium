#include "Hooks.hpp"

#include <array>
#include <cstdint>
#include <memory>
#include <subhook.h>
#include <utility>

#include "AddressInterface.hpp"

#define IMPLEMENT_HOOKS 1
#include "hooks/DrawText.hpp"

using HookEntry = std::pair<std::intptr_t, subhook::Hook **>;

static const std::unordered_map<AddressType, HookEntry> hookFunctions = {
    std::make_pair(AddressType::DrawTextFunc, std::make_pair((std::intptr_t)DrawTextHookFunc<std::int64_t[64]>, &drawTextHook))
};

static std::unordered_map<AddressType, subhook::Hook *> hooks;

void PrepareHooks()
{
    const AddressInterface::AddressTable &addressTable = GetAddressInterface()->GetAddressTable();
    for (std::size_t addressCount = 0; addressCount < (std::size_t)AddressType::_Count; addressCount++)
    {
        if (addressCount < (std::size_t)AddressType::DrawTextFunc || addressCount >= (std::size_t)AddressType::_Count)
            continue;

        if (addressTable.count((AddressType)addressCount) > 0 && hookFunctions.count((AddressType)addressCount) > 0)
        {
            HookEntry hookEntry = hookFunctions.at((AddressType)addressCount);

            subhook::Hook *hook = new subhook::Hook((void *)(GetBaseAddress() + addressTable.at((AddressType)addressCount)), (void *)hookEntry.first, subhook::HookFlag64BitOffset); // TODO: we really want to use new?
            hook->Install();
            hooks[(AddressType)addressCount] = hook;

            *hookEntry.second = hook;
        }
    }
}