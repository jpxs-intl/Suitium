#include "AddressInterfaceWin32Client.hpp"

#include <cstdint>
#include <unordered_map>
#include <utility>

#include "Version.hpp"

AddressInterfaceWin32Client::AddressInterfaceWin32Client()
{
}
AddressInterfaceWin32Client::~AddressInterfaceWin32Client()
{
}

bool AddressInterfaceWin32Client::CheckVersion() const
{
    int *gameVersionNumPtr = (int *)(GetBaseAddress() + 0x2B231C);
    char *gameVersionCharPtr = (char *)(GetBaseAddress() + 0x2B23DC);

    int actualGameVersionNum = *gameVersionNumPtr;
    char actualGameVersionChar = (*gameVersionCharPtr) + 97;

    return actualGameVersionNum == GameVersionNum && actualGameVersionChar == GameVersionChar; 
}
const std::unordered_map<AddressType, intptr_t> &AddressInterfaceWin32Client::GetAddressTable() const
{
    static std::unordered_map<AddressType, intptr_t> addressMap = {
        std::make_pair(AddressType::GameVersionNum, 0x2B231C),
        std::make_pair(AddressType::GameVersionChar, 0x2B23DC),

        std::make_pair(AddressType::DrawTextFunc, 0x6D930)
    };
    return addressMap;
}