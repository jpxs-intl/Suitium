#include "Addresses.hpp"

#include <cstdint>

#include "Address.hpp"
#include "structs/Common.hpp"
#include "structs/CSKeyboard.hpp"
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
DataAddress<structs::CSKeyboard> addresses::CSKeyboard;
DataAddress<structs::CBoolean> addresses::IsInGame;
DataAddress<structs::CBoolean> addresses::GamePaused;
DataAddress<int> addresses::MenuTypeID;
DataAddress<float> addresses::NextMenuButtonPositionX;
DataAddress<float> addresses::NextMenuButtonPositionY;
DataAddress<float> addresses::NextMenuButtonSizeX;
DataAddress<float> addresses::NextMenuButtonSizeY;
DataAddress<SDL_Scancode> addresses::NextMenuButtonKey;
DataAddress<char> addresses::ServerName;
FuncAddress<addresses::VoidFuncType> addresses::ClientMainFunc;
FuncAddress<addresses::VoidFuncType> addresses::ConnectMasterServerFunc;
FuncAddress<addresses::CreateItemFuncType> addresses::CreateItemFunc;
FuncAddress<addresses::CreateVehicleFuncType> addresses::CreateVehicleFunc;
FuncAddress<addresses::CSDrawTextFuncType> addresses::CSDrawTextFunc;
FuncAddress<addresses::VoidFuncType> addresses::LoadServerFunc;
FuncAddress<addresses::DrawMenuFuncType> addresses::DrawMenuFunc;
FuncAddress<addresses::DrawMenuButtonFuncType> addresses::DrawMenuButtonFunc;
FuncAddress<addresses::DrawMenuButtonSelectableFuncType> addresses::DrawMenuButtonSelectableFunc;
FuncAddress<addresses::PrintfFuncType> addresses::PrintfFunc;
FuncAddress<addresses::VoidFuncType> addresses::ResetGameFunc;
FuncAddress<addresses::VoidFuncType> addresses::SetupVehicleTypesFunc;

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

    addresses::ItemTypes.Register(baseAddress + DYNADDR(0x42A7E180, 0x3A92F120));
    addresses::VehicleTypes.Register(baseAddress + DYNADDR(0x52E9A680, 0x38CD0E20));
    addresses::Humans.Register(baseAddress + DYNADDR(0x4329E308, 0x5D69CE48));
    addresses::Items.Register(baseAddress + DYNADDR(0x42BBB180, 0x12A7640));
    addresses::Vehicles.Register(baseAddress + DYNADDR(0x1F2D9200, 0xA8E2480));

    addresses::AuthName.Register(baseAddress + DYNADDR(0x11E4594C, 0x1C87CFEC));
    addresses::AuthStatus.Register(baseAddress + DYNADDR(0x2B165F40, 0x34C453D8));

    addresses::SteamEnabled.Register(baseAddress + DYNADDR(0x429F8BC0, 0x1C0f67E0));
    addresses::SteamTicketRetrieved.Register(baseAddress + DYNADDR(0x429F8FD8, 0x1C0F6BF8));
    addresses::SteamTicketLength.Register(baseAddress + DYNADDR(0x429F8BD4, 0x1C0F67F4));
    addresses::SteamTicketBuffer.Register(baseAddress + DYNADDR(0x429F8BD8, 0x1C0F67F8));

    addresses::SDLWindowPtr.Register(baseAddress + DYNADDR(0x20FC47C8, 0x6A7C5F88));

    addresses::CSKeyboard.Register(baseAddress + DYNADDR(0x43F7C964, 0x0));

    addresses::IsInGame.Register(baseAddress + DYNADDR(0x43EBFAA0, 0x0));
    addresses::GamePaused.Register(baseAddress + DYNADDR(0x43EBFAA8, 0x0));
    addresses::MenuTypeID.Register(baseAddress + DYNADDR(0x43EBFAA4, 0x0));
    
    addresses::NextMenuButtonPositionX.Register(baseAddress + DYNADDR(0x10F610D8, 0x0));
    addresses::NextMenuButtonPositionY.Register(baseAddress + DYNADDR(0x10F610DC, 0x0));
    addresses::NextMenuButtonSizeX.Register(baseAddress + DYNADDR(0x10F610E0, 0x0));
    addresses::NextMenuButtonSizeY.Register(baseAddress + DYNADDR(0x10F610E4, 0x0));
    addresses::NextMenuButtonKey.Register(baseAddress + DYNADDR(0x10F610F4, 0x0));

    addresses::ServerName.Register(baseAddress + DYNADDR(0x6D04ADD4, 0x0));

    addresses::ClientMainFunc.Register(baseAddress + DYNADDR(0x96440, 0x15745C));
    addresses::ConnectMasterServerFunc.Register(baseAddress + DYNADDR(0xA3F30, 0x71447));
    addresses::CreateItemFunc.Register(baseAddress + DYNADDR(0x54130, 0xC5F2B));
    addresses::CreateVehicleFunc.Register(baseAddress + DYNADDR(0x5BCA0, 0xC92B0));
    addresses::CSDrawTextFunc.Register(baseAddress + DYNADDR(0x6D930, 0x2C62C));
    addresses::LoadServerFunc.Register(0);
    addresses::DrawMenuFunc.Register(baseAddress + DYNADDR(0x72450, 0x30821));
    addresses::DrawMenuButtonFunc.Register(baseAddress + DYNADDR(0x6F1F0, 0x2602F));
    addresses::DrawMenuButtonSelectableFunc.Register(baseAddress + DYNADDR(0x6F5A0, 0x0));
    addresses::PrintfFunc.Register(0);
    addresses::ResetGameFunc.Register(baseAddress + DYNADDR(0x127910, 0x0));
    addresses::SetupVehicleTypesFunc.Register(baseAddress + DYNADDR(0x145BB0, 0x116BD5));

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

    addresses::CSKeyboard.Register(0);

    addresses::IsInGame.Register(0);
    addresses::GamePaused.Register(0);
    addresses::MenuTypeID.Register(0);

    addresses::NextMenuButtonPositionX.Register(0);
    addresses::NextMenuButtonPositionY.Register(0);
    addresses::NextMenuButtonSizeX.Register(0);
    addresses::NextMenuButtonSizeY.Register(0);
    addresses::NextMenuButtonKey.Register(0);

    addresses::ServerName.Register(baseAddress + DYNADDR(0x23D75B74, 0x0));

    addresses::ClientMainFunc.Register(0);
    addresses::ConnectMasterServerFunc.Register(baseAddress + DYNADDR(0x4FEB0, 0x0));
    addresses::CreateItemFunc.Register(baseAddress + DYNADDR(0x3B080, 0x0));
    addresses::CreateVehicleFunc.Register(baseAddress + DYNADDR(0x414B0, 0x0));
    addresses::CSDrawTextFunc.Register(0);
    addresses::LoadServerFunc.Register(baseAddress + DYNADDR(0x8A390, 0x0));
    addresses::DrawMenuFunc.Register(0);
    addresses::DrawMenuButtonFunc.Register(0);
    addresses::DrawMenuButtonSelectableFunc.Register(0);
    addresses::PrintfFunc.Register(baseAddress + DYNADDR(0xB36F0, 0x0));
    addresses::ResetGameFunc.Register(baseAddress + DYNADDR(0xB4790, 0x0));
    addresses::SetupVehicleTypesFunc.Register(baseAddress + DYNADDR(0xCBEB0, 0x0));

    isDedicated = true;
    return true;

label_error: {}
    return false;
}