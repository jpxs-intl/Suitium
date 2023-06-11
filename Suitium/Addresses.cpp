#include "Addresses.hpp"

#include <cstdint>

#include "Address.hpp"
#include "structs/Common.hpp"
#include "structs/Human.hpp"
#include "structs/ItemType.hpp"
#include "structs/Vehicle.hpp"
#include "structs/VehicleType.hpp"
#include "Version.hpp"

DataAddress<void> addresses::Base;
DataAddress<bool> addresses::IsDedicated;
DataAddress<const int> addresses::GameVersionNumber;
DataAddress<const std::uint8_t> addresses::GameVersionPatchNumber;
DataAddress<structs::ItemType> addresses::ItemTypes;
DataAddress<structs::VehicleType> addresses::VehicleTypes;
DataAddress<structs::Human> addresses::Humans;
DataAddress<structs::Item> addresses::Items;
DataAddress<structs::Vehicle> addresses::Vehicles;
DataAddress<char> addresses::AuthName;
DataAddress<int> addresses::AuthStatus;
DataAddress<structs::CBoolean> addresses::SteamEnabled;
DataAddress<structs::CBoolean> addresses::SteamTicketRetrieved;
DataAddress<int> addresses::SteamTicketLength;
DataAddress<char> addresses::SteamTicketBuffer;
DataAddress<void *> addresses::SDLWindowPtr;
FuncAddress<addresses::ConnectMasterServerFuncType> addresses::ConnectMasterServerFunc;
FuncAddress<addresses::CreateItemFuncType> addresses::CreateItemFunc;
FuncAddress<addresses::CreateVehicleFuncType> addresses::CreateVehicleFunc;
FuncAddress<addresses::CSDrawTextFuncType> addresses::CSDrawTextFunc;
FuncAddress<addresses::VoidFuncType> addresses::LoadServerFunc;
FuncAddress<addresses::MainMenuType> addresses::MainMenuFunc;
FuncAddress<addresses::VoidFuncType> addresses::ClientMain;
FuncAddress<addresses::PrintfFuncType> addresses::PrintfFunc;
FuncAddress<addresses::SetupVehicleTypesFuncType> addresses::SetupVehicleTypesFunc;

static bool isDedicated = false;

bool addresses::Map(std::uintptr_t baseAddress)
{
    addresses::Base.Register(baseAddress);
    addresses::IsDedicated.Register((std::uintptr_t)&isDedicated);

label_client: {}
    addresses::GameVersionNumber.Register(baseAddress + DYNADDR(0x2B231C, 0x389888));
    addresses::GameVersionPatchNumber.Register(baseAddress + DYNADDR(0x2B23DC, 0x389884));

    if (*addresses::GameVersionNumber.ptr != SUITIUM_GAME_VERSION_NUMBER || *addresses::GameVersionPatchNumber.ptr != SUITIUM_GAME_VERSION_PATCH - 97)
        goto label_dedicated;

    addresses::ItemTypes.Register(baseAddress + DYNADDR(0x42A7E180, 0x3a92f120));
    addresses::VehicleTypes.Register(baseAddress + DYNADDR(0x52E9A680, 0x38cd0e20));
    addresses::Humans.Register(baseAddress + DYNADDR(0x4329E308, 0x5d69ce48));
    addresses::Items.Register(baseAddress + DYNADDR(0x42BBB180, 0x12a7640));
    addresses::Vehicles.Register(baseAddress + DYNADDR(0x1F2D9200, 0xa8e2480));

    addresses::AuthName.Register(baseAddress + DYNADDR(0x11E4594C, 0x1c87cfec));
    addresses::AuthStatus.Register(baseAddress + DYNADDR(0x2B165F40, 0x34c453d8));

    addresses::SteamEnabled.Register(baseAddress + DYNADDR(0x429F8BC0, 0x1c0f67e0));
    addresses::SteamTicketRetrieved.Register(baseAddress + DYNADDR(0x429F8FD8, 0x1c0f6bf8));
    addresses::SteamTicketLength.Register(baseAddress + DYNADDR(0x429F8BD4, 0x1c0f67f4));
    addresses::SteamTicketBuffer.Register(baseAddress + DYNADDR(0x429F8BD8, 0x1c0f67f8));

    addresses::SDLWindowPtr.Register(baseAddress + DYNADDR(0x20FC47C8, 0x6a7c5f88));

    addresses::ServerName.Register(baseAddress + DYNADDR(0x6D04ADD4, 0x0));

    addresses::ConnectMasterServerFunc.Register(baseAddress + DYNADDR(0xA3F30, 0x71447));
    addresses::CreateItemFunc.Register(baseAddress + DYNADDR(0x54130, 0xc5f2b));
    addresses::CreateVehicleFunc.Register(baseAddress + DYNADDR(0x5BCA0, 0xc92b0));
    addresses::CSDrawTextFunc.Register(baseAddress + DYNADDR(0x6D930, 0x2c62c));
    addresses::LoadServerFunc.Register(0);
    addresses::ClientMain.Register(baseAddress + DYNADDR(0x96440, 0x15745c));
    addresses::MainMenu.Register(baseAddress + DYNADDR(0x72450, 0x30821));
    addresses::PrintfFunc.Register(0);
    addresses::ResetGameFunc.Register(baseAddress + DYNADDR(0x127910, 0x0));
    addresses::SetupVehicleTypesFunc.Register(baseAddress + DYNADDR(0x145BB0, 0x116bd5));

    isDedicated = false;
    return true;

label_dedicated: {}
    addresses::GameVersionNumber.Register(baseAddress + DYNADDR(0xF7ADC, 0x0)); // This is probably not the real address, but the only read only data I could find containing the right value
    addresses::GameVersionPatchNumber.Register(baseAddress + DYNADDR(0xF79DC, 0x0));  // This is probably not the real address, but the only read only data I could find containing the right value
    if (*addresses::GameVersionNumber.ptr != SUITIUM_GAME_VERSION_NUMBER || *addresses::GameVersionPatchNumber.ptr != SUITIUM_GAME_VERSION_PATCH_DEDICATED - 97)
        goto label_error;

    addresses::ItemTypes.Register(baseAddress + DYNADDR(0x501220C0, 0x0));
    addresses::VehicleTypes.Register(baseAddress + DYNADDR(0xAA152E0, 0x0));
    addresses::Humans.Register(baseAddress + DYNADDR(0x509419C8, 0x0));
    addresses::Items.Register(baseAddress + DYNADDR(0x5025F0C0, 0x0));
    addresses::Vehicles.Register(baseAddress + DYNADDR(0x2E6A3C60, 0x0));

    addresses::AuthName.Register(0);
    addresses::AuthStatus.Register(0);

    addresses::SteamEnabled.Register(0);
    addresses::SteamTicketRetrieved.Register(0);
    addresses::SteamTicketLength.Register(0);
    addresses::SteamTicketBuffer.Register(0);

    addresses::SDLWindowPtr.Register(0);

    addresses::ServerName.Register(baseAddress + DYNADDR(0x23D75B74, 0x0));

    addresses::ConnectMasterServerFunc.Register(baseAddress + DYNADDR(0x4FEB0, 0x0));
    addresses::CreateItemFunc.Register(baseAddress + DYNADDR(0x3B080, 0x0));
    addresses::CreateVehicleFunc.Register(baseAddress + DYNADDR(0x414B0, 0x0));
    addresses::CSDrawTextFunc.Register(0);
    addresses::LoadServerFunc.Register(baseAddress + DYNADDR(0x8A390, 0x0));
    addresses::MainMenuFunc.Register(0);
    addresses::ClientMain.Register(0);
    addresses::PrintfFunc.Register(baseAddress + DYNADDR(0xB36F0, 0x0));
    addresses::ResetGameFunc.Register(baseAddress + DYNADDR(0xB4790, 0x0));
    addresses::SetupVehicleTypesFunc.Register(baseAddress + DYNADDR(0xCBEB0, 0x0));

    isDedicated = true;
    return true;

label_error: {}
    return false;
}