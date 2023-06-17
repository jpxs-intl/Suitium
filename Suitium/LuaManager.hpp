#pragma once

#include <memory>
#include <sol/sol.hpp>

#define LUAMANAGER_LUAADDONINDEX 500

class Addon;

class LuaManager
{
public:
    LuaManager();
    LuaManager(const LuaManager &) = delete;
    ~LuaManager() noexcept = default;

    void Initialize();
    void Deinitialize();

private:
    std::unique_ptr<sol::state> _L;
public:
    sol::state *L() const;

    Addon *GetCurrentAddon();
    void SetCurrentAddon(Addon *addon);
};

LuaManager *GetMainLuaManager();