#include "Addresses.hpp"

#include <cstdint>

#include "Address.hpp"
#include "structs/Common.hpp"
#include "structs/ItemType.hpp"
#include "structs/VehicleType.hpp"
#include "Version.hpp"

DataAddress<bool> addresses::IsDedicated;
DataAddress<int> addresses::GameVersionNumber;
DataAddress<std::uint8_t> addresses::GameVersionPatchNumber;
DataAddress<structs::ItemType> addresses::ItemTypes;
DataAddress<structs::VehicleType> addresses::VehicleTypes;
DataAddress<char> addresses::AuthName;
DataAddress<int> addresses::AuthStatus;
DataAddress<structs::CBoolean> addresses::SteamEnabled;
DataAddress<structs::CBoolean> addresses::SteamTicketRetrieved;
DataAddress<int> addresses::SteamTicketLength;
DataAddress<char> addresses::SteamTicketBuffer;
DataAddress<void *> addresses::SDLWindowPtr;
FuncAddress<addresses::CSDrawTextFuncType> addresses::CSDrawTextFunc;
FuncAddress<addresses::MainMenuFuncType> addresses::MainMenuFunc;
FuncAddress<addresses::ConnectMasterServerFuncType> addresses::ConnectMasterServerFunc;

static bool isDedicated = false;

bool addresses::MapForWin32(std::uintptr_t baseAddress)
{
    addresses::IsDedicated.Register((std::uintptr_t)&isDedicated);

label_client: {}
    addresses::GameVersionNumber.Register(baseAddress + 0x2B231C);
    addresses::GameVersionPatchNumber.Register(baseAddress + 0x2B23DC);
    if (*addresses::GameVersionNumber.ptr != SUITIUM_GAME_VERSION_NUMBER || *addresses::GameVersionPatchNumber.ptr != SUITIUM_GAME_VERSION_PATCH - 97)
        goto label_dedicated;

    addresses::ItemTypes.Register(baseAddress + 0x42A7E180);
    addresses::VehicleTypes.Register(baseAddress + 0x52E9A680);
    addresses::Humans.Register(baseAddress + 0x4329E308);

    addresses::AuthName.Register(baseAddress + 0x11E4594C);
    addresses::AuthStatus.Register(baseAddress + 0x2B165F40);

    addresses::SteamEnabled.Register(baseAddress + 0x429F8BC0);
    addresses::SteamTicketRetrieved.Register(baseAddress + 0x429F8FD8);
    addresses::SteamTicketLength.Register(baseAddress + 0x429F8BD4);
    addresses::SteamTicketBuffer.Register(baseAddress + 0x429F8BD8);

    addresses::SDLWindowPtr.Register(baseAddress + 0x20FC47C8);

    addresses::ConnectMasterServerFunc.Register(baseAddress + 0xA3F30);
    addresses::MainMenuFunc.Register(baseAddress + 0x96440);
    addresses::CSDrawTextFunc.Register(baseAddress + 0x6D930);

    isDedicated = false;
    return true;

label_dedicated: {}
    goto label_error; // TODO:

    isDedicated = true;
    return true;

label_error: {}
    return false;
}
bool addresses::MapForLinux(std::uintptr_t baseAddress)
{
    addresses::IsDedicated.Register((std::uintptr_t)&isDedicated);

    return false; // TODO:
}