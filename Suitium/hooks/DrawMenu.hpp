#pragma once

#include <fmt/format.h>
#include <glm/glm.hpp>
#include <subhook.h>

#include "../Addresses.hpp"
#include "../api/Text.hpp"
#include "../Version.hpp"

static subhook::Hook *drawMenuHook;

int DrawMenuHookFunc(int unk);

int DrawMenuHookFunc(int unk)
{
    subhook::ScopedHookRemove scopedRemove(drawMenuHook);

    int returnValue = addresses::DrawMenuFunc(unk);

    if (!*addresses::IsInGame && *addresses::MenuTypeID == 0)
    {
        // The main menu is being drawn!
        api::DrawText(fmt::format("Sub Rosa 0.{}{}", SUITIUM_GAME_VERSION_NUMBER, SUITIUM_GAME_VERSION_PATCH), 512.0f, 128.0f, 24.0f, glm::vec4(1.0f));    
        api::DrawText(fmt::format("Suitium {}", SUITIUM_VERSION), 512.0f, 128.0f + 22.0f, 16.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));    
    }

    return returnValue;
}