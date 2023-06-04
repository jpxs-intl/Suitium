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
    char actualGameVersionChar = (*gameVersionCharPtr) + 97; // 97 is 'a', CS uses numbers to define the patch version, but they are converted to letters.

    return actualGameVersionNum == GameVersionNum && actualGameVersionChar == GameVersionChar; 
}
const AddressInterface::AddressMap &AddressInterfaceWin32Client::GetAddressMap() const
{
    static AddressInterface::AddressMap addressMap = {
        std::make_pair(AddressType::GameVersionNum, 0x2B231C),
        std::make_pair(AddressType::GameVersionChar, 0x2B23DC),

        std::make_pair(AddressType::AuthName, 0x11E4594C),
        std::make_pair(AddressType::AuthStatus, 0x2B165F40),
        std::make_pair(AddressType::AuthTicketRetrieved, 0x429F8FD8),

        std::make_pair(AddressType::SteamEnabled, 0x429F8BC0),
        std::make_pair(AddressType::SteamAppTicketLength, 0x429F8BD4),  
        std::make_pair(AddressType::SteamAppTicketBuffer, 0x429F8BD8),

        std::make_pair(AddressType::DrawTextFunc, 0x6D930),
        std::make_pair(AddressType::ConnectMasterServerFunc, 0xA3F30)
    };
    return addressMap;
}