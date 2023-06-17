#pragma once

#include <memory>
#include <sol/sol.hpp>
#include <string>
#include <vector>

#include "LuaManager.hpp"

class Addon
{
public:
    Addon() = delete;
    Addon(const std::string &folderPath, bool asSR = false);
    ~Addon() noexcept;

private:
    std::string _folderPath;
    bool _asSR;

public:
    bool IsLoaded() const;
    void Load();
    void Unload();
private:
    void LoadAsSR();

    bool _isLoaded;

    std::string _id;
public:
    const std::string& ID() const;

    std::string _name;
    std::string _description;
    std::string _logDecoration;
public:
    const std::string& Name() const;
    const std::string& Description() const;
    const std::string& LogDecoration() const;

private:
    std::vector<std::string> _requires;
    std::vector<std::string> _conflicts;
public:
    bool CheckDependencies();

private:
    std::unique_ptr<sol::thread> _addonThread;
public:
    bool PrepareLua(LuaManager *manager);
};

void DiscoverAddons();
const std::vector<std::unique_ptr<Addon>> &GetAddons();
Addon *FindAddon(const std::string &addonID);