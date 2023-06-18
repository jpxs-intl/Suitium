#pragma once

#include <glad/glad.h>
#include <subhook.h>

extern subhook::Hook *clientMainHook;

void ClientMainHookFunc();

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include "../Addon.hpp"
#include "../Addresses.hpp"
#include "../LuaManager.hpp"
#include "../structs/ItemType.hpp"
#include "../structs/VehicleType.hpp"

subhook::Hook *clientMainHook;

#if _WIN32
#include <windows.h>
#elif __linux__
#include <dlfcn.h>
#endif

typedef struct SDL_RWops SDL_RWops_t;
typedef SDL_RWops_t *(*SDL_RWFromFile_t)(const char *filePath, const char *mode);

typedef struct SDL_Surface SDL_Surface_t;
typedef SDL_Surface_t *(*SDL_LoadBMP_RW_t)(SDL_RWops_t *src, int freeSrc);
typedef void (*SDL_FreeSurface_t)(SDL_Surface_t *surface);

typedef struct SDL_Window SDL_Window_t;
typedef void (*SDL_SetWindowIcon_t)(SDL_Window_t *window, SDL_Surface_t *icon);

void ClientMainHookFunc()
{
    subhook::ScopedHookRemove scopedRemove(clientMainHook);

#if _WIN32
    HMODULE sdlModule = GetModuleHandle("SDL2.dll");

    gladLoadGLLoader((GLADloadproc)GetProcAddress(sdlModule, "SDL_GL_GetProcAddress"));

    SDL_RWFromFile_t sdlRWFromFile = (SDL_RWFromFile_t)GetProcAddress(sdlModule, "SDL_RWFromFile");
    SDL_LoadBMP_RW_t sdlLoadBMP_RW = (SDL_LoadBMP_RW_t)GetProcAddress(sdlModule, "SDL_LoadBMP_RW");
    SDL_FreeSurface_t sdlFreeSurface = (SDL_FreeSurface_t)GetProcAddress(sdlModule, "SDL_FreeSurface");
    SDL_SetWindowIcon_t sdlSetWindowIcon = (SDL_SetWindowIcon_t)GetProcAddress(sdlModule, "SDL_SetWindowIcon");
    // Maybe we should check if Windows could actually found the functions, but whatever

    // SDL_LoadBMP is a macro (not a function) defined like
    // #define SDL_LoadBMP(filePath) SDL_LoadBMP_RW(SDL_RWFromFile(filePath, "r"), 1)
    SDL_Surface_t *iconBMP = sdlLoadBMP_RW(sdlRWFromFile("subrosa.bmp", "r"), 1);
    if (iconBMP)
    {
        sdlSetWindowIcon((SDL_Window_t *)*addresses::SDLWindowPtr, iconBMP);
        sdlFreeSurface(iconBMP);
    }
#else
    void* sdlModule = dlopen("libSDL2-2.0.so.0", RTLD_LAZY | RTLD_NOLOAD);

    gladLoadGLLoader((GLADloadproc)dlsym(sdlModule, "SDL_GL_GetProcAddress"));

    SDL_RWFromFile_t sdlRWFromFile = (SDL_RWFromFile_t)dlsym(sdlModule, "SDL_RWFromFile");
    SDL_LoadBMP_RW_t sdlLoadBMP_RW = (SDL_LoadBMP_RW_t)dlsym(sdlModule, "SDL_LoadBMP_RW");
    SDL_FreeSurface_t sdlFreeSurface = (SDL_FreeSurface_t)dlsym(sdlModule, "SDL_FreeSurface");
    SDL_SetWindowIcon_t sdlSetWindowIcon = (SDL_SetWindowIcon_t)dlsym(sdlModule, "SDL_SetWindowIcon");

    // SDL_LoadBMP is a macro (not a function) defined like
    // #define SDL_LoadBMP(filePath) SDL_LoadBMP_RW(SDL_RWFromFile(filePath, "r"), 1)
    SDL_Surface_t *iconBMP = sdlLoadBMP_RW(sdlRWFromFile("subrosa.bmp", "r"), 1);
    if (iconBMP)
    {
      sdlSetWindowIcon((SDL_Window_t *)*addresses::SDLWindowPtr, iconBMP);
      sdlFreeSurface(iconBMP);
    }
#endif

    GetMainLuaManager()->Initialize();

    DiscoverAddons();
    for (auto it = GetAddons().begin(); it != GetAddons().end(); ++it)
        (*it)->Load(); // This loads in the ConnectMasterServer hook for the dedicated
    for (auto it = GetAddons().begin(); it != GetAddons().end(); ++it)
    {
        if (!(*it)->IsLoaded())
            continue;
        if ((*it)->CheckDependencies())
            (*it)->PrepareLua(GetMainLuaManager(), false);
    }

    // We gotta setup the structs!
    for (std::size_t itemTypeCount = 0; itemTypeCount < structs::ItemType::VanillaCount; itemTypeCount++)
    {
        addresses::ItemTypes[itemTypeCount].customData.index = itemTypeCount;
        addresses::ItemTypes[itemTypeCount].customData.typeIDPtr = new std::string();
    }
    for (std::size_t vehicleTypeCount = 0; vehicleTypeCount < structs::VehicleType::VanillaCount; vehicleTypeCount++)
    {
        addresses::VehicleTypes[vehicleTypeCount].customData.index = vehicleTypeCount;
        addresses::VehicleTypes[vehicleTypeCount].customData.typeIDPtr = new std::string();
    }

    addresses::ClientMainFunc();

    for (auto it = GetAddons().begin(); it != GetAddons().end(); ++it)
        (*it)->Unload();
    
    GetMainLuaManager()->Deinitialize();
}

#endif