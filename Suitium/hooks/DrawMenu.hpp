#pragma once

int DrawMenuHookFunc(int unk);

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include <fmt/format.h>
#include <glm/glm.hpp>
#include <subhook.h>

#include "../Addon.hpp"
#include "../Addresses.hpp"
#include "../api/Text.hpp"
#include "../structs/CSKeyboard.hpp"
#include "../Version.hpp"

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
        *addresses::NextMenuButtonKey = (SDL_Scancode)-1;
        if (addresses::DrawMenuButtonFunc("Addons"))
            *addresses::MenuTypeID = 100 + 0; // 100 is the base ID for suitium menus
    }
    else if (!*addresses::IsInGame && *addresses::MenuTypeID >= 100)
    {
        int customMenuTypeID = *addresses::MenuTypeID - 100;
        if (customMenuTypeID == 0)
        {
            static int selectedAddonID = -1;

            *addresses::NextMenuButtonPositionX = 4.0f;
            *addresses::NextMenuButtonPositionY = 4.0f;
            *addresses::NextMenuButtonSizeX = 160.0f;
            *addresses::NextMenuButtonSizeY = 20.0f;
            *addresses::NextMenuButtonKey = SDL_SCANCODE_ESCAPE;
            if (addresses::DrawMenuButtonFunc("Back"))
            {
                *addresses::MenuTypeID = 0;
                selectedAddonID = -1;
            }
            
            {
                std::size_t addonCount = 0;
                for (auto it = GetAddons().begin(); it != GetAddons().end(); it++)
                {
                    *addresses::NextMenuButtonPositionX = 8.0f;
                    *addresses::NextMenuButtonPositionY = 128.0f + ((float)addonCount * (*addresses::NextMenuButtonSizeY + 2.0f));
                    *addresses::NextMenuButtonSizeX = 192.0f;
                    *addresses::NextMenuButtonSizeY = 48.0f;
                    *addresses::NextMenuButtonKey = (SDL_Scancode)-1;
                    addresses::DrawMenuButtonSelectableFunc("", &selectedAddonID, (int)addonCount);

                    addonCount++;
                }
            }

            int returnValue = addresses::DrawMenuFunc(unk);

            {
                api::DrawText("Addons:", 10.0f, 128.0f - 24.0f, 22.0f, glm::vec4(1.0f), api::TextAlignment::Right);

                std::size_t addonCount = 0;
                for (auto it = GetAddons().begin(); it != GetAddons().end(); it++)
                {
                    const std::size_t maxNumberOfDisplayChars = 15; // must be bigger than 3
                    std::string name = (*it)->Name();
                    if (name.size() > maxNumberOfDisplayChars)
                    {
                        name.erase(name.begin() + (maxNumberOfDisplayChars - 3), name.end());
                        name.push_back('.');
                        name.push_back('.');
                        name.push_back('.');
                    }

                    *addresses::NextMenuButtonPositionX = 8.0f;
                    *addresses::NextMenuButtonPositionY = 128.0f + ((float)addonCount * (*addresses::NextMenuButtonSizeY + 2.0f));
                    *addresses::NextMenuButtonSizeX = 192.0f;
                    *addresses::NextMenuButtonSizeY = 48.0f;
                    *addresses::NextMenuButtonKey = (SDL_Scancode)-1;
                    api::DrawText(name.c_str(), *addresses::NextMenuButtonPositionX + (*addresses::NextMenuButtonSizeX * 0.35f), *addresses::NextMenuButtonPositionY + ((*addresses::NextMenuButtonSizeY * 0.5f) - 8.0f), 16.0f, glm::vec4(1.0f), api::TextAlignment::Right);

                    addonCount++;
                }
            }

            if (selectedAddonID >= 0)
            {
                api::DrawText(fmt::format("{} ({})", GetAddons()[selectedAddonID]->Name(), GetAddons()[selectedAddonID]->ID()), 256.0f, 128.0f, 22.0f, glm::vec4(1.0f), api::TextAlignment::Right);
                api::DrawText(GetAddons()[selectedAddonID]->Description(), 256.0f, 128.0f + 24.0f, 22.0f, glm::vec4(1.0f), api::TextAlignment::Right);
            }

            return returnValue;
        }
    }

    return addresses::DrawMenuFunc(unk);
}

#endif