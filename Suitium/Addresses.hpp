#pragma once

#include <cstdint>

#include "Address.hpp"
#include "structs/Common.hpp"
#include "structs/CSKeyboard.hpp"
#include "structs/CSTexture.hpp"
#include "structs/Human.hpp"
#include "structs/Item.hpp"
#include "structs/ItemType.hpp"
#include "structs/Vehicle.hpp"
#include "structs/VehicleType.hpp"

namespace addresses
{
    extern DataAddress<void> Base;
    extern DataAddress<bool> IsDedicated;

    extern DataAddress<const int> GameVersionNumber;
    extern DataAddress<const std::uint8_t> GameVersionPatchNumber;

    extern DataAddress<structs::ItemType> ItemTypes;
    extern DataAddress<structs::VehicleType> VehicleTypes;
    extern DataAddress<structs::Human> Humans;
    extern DataAddress<structs::Item> Items;
    extern DataAddress<structs::Vehicle> Vehicles;

    extern DataAddress<char> AuthName;
    extern DataAddress<int> AuthStatus;

    extern DataAddress<structs::CBoolean> SteamEnabled;
    extern DataAddress<structs::CBoolean> SteamTicketRetrieved;
    extern DataAddress<int> SteamTicketLength;
    extern DataAddress<char> SteamTicketBuffer;

    extern DataAddress<void *> SDLWindowPtr;

    extern DataAddress<structs::CSKeyboard> CSKeyboard;
    extern DataAddress<structs::CSTexture> CSTextures;

    extern DataAddress<structs::CBoolean> IsInGame;
    extern DataAddress<structs::CBoolean> GamePaused;
    extern DataAddress<int> MenuOptionsSectionID;
    extern DataAddress<int> MenuTypeID;

    extern DataAddress<float> NextMenuButtonPositionX;
    extern DataAddress<float> NextMenuButtonPositionY;
    extern DataAddress<float> NextMenuButtonSizeX;
    extern DataAddress<float> NextMenuButtonSizeY;
    extern DataAddress<SDL_Scancode> NextMenuButtonKey;

    extern DataAddress<char> ServerName;

    using VoidFuncType = void ();

    extern FuncAddress<VoidFuncType> ClientMainFunc;

    extern FuncAddress<VoidFuncType> ConnectMasterServerFunc;

    using CreateItemFuncType = int (int typeID, structs::CVector3 *position, structs::CVector3 *velocity, structs::COrientation *orientation);
    extern FuncAddress<CreateItemFuncType> CreateItemFunc;

    using CreateVehicleFuncType = int (int typeID, structs::CVector3 *position, structs::CVector3 *velocity, structs::COrientation *orientation, int colorID);
    extern FuncAddress<CreateVehicleFuncType> CreateVehicleFunc;

    using CSDrawAtlasEntryFuncType = void (int atlasID, int atlasEntryID, float x, float y, float width, float height, char unk1, float r, float g, float b, float a);
    extern FuncAddress<CSDrawAtlasEntryFuncType> CSDrawAtlasEntryFunc;

#if _WIN32
    using CSDrawTextFuncType = std::int64_t (const char *format, float x, float y, float size, unsigned int flags, float red, float green, float blue, float alpha, ...);
#elif __linux__
    using CSDrawTextFuncType = std::int64_t (const char *format, int, int, int, float, float, float, float, float, float, float, void *);
#endif
    extern FuncAddress<CSDrawTextFuncType> CSDrawTextFunc;

    using DrawMenuFuncType = int (int unk);
    extern FuncAddress<DrawMenuFuncType> DrawMenuFunc;

    using DrawMenuButtonFuncType = int (const char *text);
    extern FuncAddress<DrawMenuButtonFuncType> DrawMenuButtonFunc;

    using DrawMenuButtonSelectableFuncType = int (const char *text, int *selected, int selectedCmp);
    extern FuncAddress<DrawMenuButtonSelectableFuncType> DrawMenuButtonSelectableFunc;

#if _WIN32
    using DrawMenuListFuncType = int (const char *text, int unk1, int unk2, int unk3, int unk4, int unk5, void *unk6, void *unk7, int unk8, int unk9);
#elif __linux__
    using DrawMenuListFuncType = int (const char *text, std::uint64_t unk1, void *unk2, void *unk3, std::uint64_t unk4, unsigned int *unk5, float unk6, float unk7, float unk8, float unk9);
#endif
    extern FuncAddress<DrawMenuListFuncType> DrawMenuListFunc;

    using DrawMenuSliderFuncType = int (const char *text, int *value, int minValue, int maxValue, int unk);
    extern FuncAddress<DrawMenuSliderFuncType> DrawMenuSliderFunc;

    using DrawMenuTextBoxFuncType = int (const char *text, int *value, int unk1, int unk2);
    extern FuncAddress<DrawMenuTextBoxFuncType> DrawMenuTextBoxFunc;

    using DrawMenuToggleFuncType = int (const char *text, int *value);
    extern FuncAddress<DrawMenuToggleFuncType> DrawMenuToggleFunc;

    using PrintfFuncType = int (const char *format, ...);
    extern FuncAddress<PrintfFuncType> PrintfFunc;

    extern FuncAddress<VoidFuncType> ResetGameFunc;

    extern FuncAddress<VoidFuncType> ServerMainFunc;

    extern FuncAddress<VoidFuncType> SetupItemTypesFunc;
    extern FuncAddress<VoidFuncType> SetupVehicleTypesFunc;

    bool Map(std::uintptr_t baseAddress);
}