#include "Addresses.hpp"

#include "Address.hpp"
#include "Version.hpp"

DataAddress<int> addresses::GameVersionNumber;
DataAddress<std::uint8_t> addresses::GameVersionPatchNumber;
DataAddress<char> addresses::AuthName;
DataAddress<int> addresses::AuthStatus;
DataAddress<int> addresses::SteamEnabled;
DataAddress<int> addresses::SteamTicketRetrieved;
DataAddress<int> addresses::SteamTicketLength;
DataAddress<char> addresses::SteamTicketBuffer;
FuncAddress<addresses::DrawTextFuncType> addresses::DrawTextFunc;
FuncAddress<addresses::ConnectMasterServerFuncType> addresses::ConnectMasterServerFunc;

bool MapAddressesForWin32(std::uintptr_t baseAddress)
{
label_client: {}

    addresses::GameVersionNumber.Register(baseAddress + 0x2B231C);
    addresses::GameVersionPatchNumber.Register(baseAddress + 0x2B23DC);
    if (*addresses::GameVersionNumber.ptr != GameVersionNumber || *addresses::GameVersionPatchNumber.ptr != GameVersionPatch - 97)
        goto label_dedicated;

    addresses::AuthName.Register(baseAddress + 0x11E4594C);
    addresses::AuthStatus.Register(baseAddress + 0x2B165F40);
    addresses::SteamEnabled.Register(baseAddress + 0x429F8BC0);
    addresses::SteamTicketLength.Register(baseAddress + 0x429F8BD4);
    addresses::SteamTicketBuffer.Register(baseAddress + 0x429F8BD8);
    addresses::SteamTicketRetrieved.Register(baseAddress + 0x429F8FD8);
    addresses::ConnectMasterServerFunc.Register(baseAddress + 0xA3F30);
    addresses::DrawTextFunc.Register(baseAddress + 0x6D930);

    return true;

label_dedicated: {}
    goto label_error; // TODO:

    return true;

label_error: {}
    return false;
}
bool MapAddressesForLinux(std::uintptr_t baseAddress)
{
    return false; // TODO:
}