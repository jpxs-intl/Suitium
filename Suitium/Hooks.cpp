#include "Hooks.hpp"

#include <array>
#include <cstdint>
#include <memory>
#include <subhook.h>

#include "AddressInterface.hpp"

static std::array<std::unique_ptr<subhook::Hook>, (std::size_t)AddressType::_Count> hooks;

void PrepareHooks()
{
    const std::unordered_map<AddressType, intptr_t> &addressTable = GetAddressInterface()->GetAddressTable();
    for (std::size_t addressCount = 0; addressCount < (std::size_t)AddressType::_Count; addressCount++)
    {
        if (addressCount < (std::size_t)AddressType::DrawTextFunc || addressCount >= (std::size_t)AddressType::_Count)
            continue;

        // TODO: hooking
    }
}