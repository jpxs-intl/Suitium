#pragma once

#include <fmt/format.h>
#include <glm/glm.hpp>
#include <subhook.h>

#include "../Addresses.hpp"
#include "../api/Text.hpp"
#include "../Version.hpp"

int DrawMenuHookFunc(int unk);

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

static subhook::Hook *drawMenuHook;

int DrawMenuHookFunc(int unk)
{
    subhook::ScopedHookRemove scopedRemove(drawMenuHook);

    if (!*addresses::IsInGame && *addresses::MenuTypeID == 0)
    {
        // The main menu is being drawn!
        api::DrawText(fmt::format("Sub Rosa 0.{}{}", SUITIUM_GAME_VERSION_NUMBER, SUITIUM_GAME_VERSION_PATCH), 512.0f, 128.0f, 24.0f, glm::vec4(1.0f));    
        api::DrawText(fmt::format("Suitium {}", SUITIUM_VERSION), 512.0f, 128.0f + 22.0f, 16.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));    
        
        *addresses::NextMenuButtonPositionX = 16.0f;
        *addresses::NextMenuButtonPositionY = 464.0f + 26.0f;
        *addresses::NextMenuButtonSizeX = 192.0f;
        *addresses::NextMenuButtonSizeY = 24.0f;

        int addonsClicked = addresses::DrawMenuButtonFunc("Addons");
        if (addonsClicked)
            *addresses::MenuTypeID = 100 + 0; // 100 is the base ID for suitium menus
    }
    else if (!*addresses::IsInGame && *addresses::MenuTypeID >= 100)
    {
        
    }

    return addresses::DrawMenuFunc(unk);
}

#endif