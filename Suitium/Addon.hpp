#pragma once

#include <memory>
#include <string>
#include <vector>

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
    bool _isLoaded;

    std::string _id;
    std::string _logDecoration;

    std::vector<std::string> _requires;
    std::vector<std::string> _conflicts;

public:
    const std::string &ID() const;
    const std::string &LogDecoration() const;

private:
    void LoadAsSR();

public:
    bool CheckDependencies();
};

void DiscoverAddons();
const std::vector<std::unique_ptr<Addon>> &GetAddons();
Addon *FindAddon(const std::string &addonID);