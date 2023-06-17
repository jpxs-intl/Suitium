#pragma once

#include <algorithm>
#include <memory>
#include <sol/sol.hpp>
#include <stdexcept>
#include <utility>

#define LUAMANAGER_LUAADDONINDEX 500

class Addon;

struct LuaHook
{
    bool active;
    Addon *addon;
    std::string name;
    sol::protected_function function;
    std::vector<std::string> flags;

    void Remove()
    {
        if (!this->active)
            throw std::logic_error("Hook was already removed");
        this->active = false;
    }
};

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

private:
    std::vector<std::shared_ptr<LuaHook>> _hooks;
public:
    const std::vector<std::shared_ptr<LuaHook>> &GetHooks() const;
    void CheckHooks();

    template<typename... Args>
    void CallHooks(const std::string &hookName, const std::string &hookFlag, Args&&... args)
    {
        for (const std::shared_ptr<LuaHook> &hook : this->GetHooks())
        {
            if (hook->active && hook->name == hookName && std::find(hook->flags.begin(), hook->flags.end(), hookFlag) != hook->flags.end())
            {
                this->SetCurrentAddon(hook->addon);
                hook->function(std::forward<Args>(args)...);
                if (std::find(hook->flags.begin(), hook->flags.end(), "once") != hook->flags.end())
                    hook->Remove();
            }
        }

        this->CheckHooks();
    }
};

LuaManager *GetMainLuaManager();