#include "Addon.hpp"

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <memory>
#include <stdexcept>

#include "api/Logging.hpp"
#include "LuaManager.hpp"

static std::vector<std::unique_ptr<Addon>> addons;

Addon::Addon(const std::string &folderPath, int asType)
{
    this->_folderPath = folderPath;
    this->_asType = asType;

    for (auto it = this->_folderPath.begin(); it != this->_folderPath.end(); ++it)
    {
        if (*it == '\\') // This looks ugly on Windows
            *it = '/';
    }

    this->_isLoaded = false;
}
Addon::~Addon()
{
    if (this->IsLoaded())
        this->Unload();
}

bool Addon::IsLoaded() const
{
    return this->_isLoaded;
}
void Addon::Load()
{
    if (this->IsLoaded())
    {
        api::GetSuitiumLogger()->Log("<yellow>Tried to reload addon \"{}\" while it's already loaded.<reset> Did you forget to unload it first?", this->ID());
        return;
    }

    if (this->_asType >= 0)
    {
        if (this->_asType == 0)
            this->LoadAsSR();
        else if (this->_asType == 1)
            this->LoadAsSuitium();
        return;
    }

    api::GetSuitiumLogger()->Log("Loading addon folder \"{}\"...", this->_folderPath);

    std::string errorMessage = "";

    {
        std::string addonsJSONPath = this->_folderPath;
        addonsJSONPath.append("/addon.json");

        std::ifstream addonsJSONFile(addonsJSONPath);
        if (!addonsJSONFile.good())
        {
            errorMessage = "Could not open addon.json";
            goto label_error;
        }

        nlohmann::json addonsJSON;
        try
        {
            addonsJSON = nlohmann::json::parse(addonsJSONFile);
        } catch (...)
        {
            errorMessage = "Could not parse addon.json";
            goto label_error;
        }

        auto jsonID = addonsJSON["id"];
        if (!jsonID.is_string())
        {
            errorMessage = "addon.json does not contain ID field or it's invalid";
            goto label_error;
        }
        this->_id = jsonID.get<std::string>();

        if (this->_id == "sub_rosa")
        {
            errorMessage = "Addon uses a reserved ID \"sub_rosa\"";
            goto label_error;
        }
        if (this->_id == "suitium")
        {
            errorMessage = "Addon uses a reserved ID \"suitium\"";
            goto label_error;
        }

        this->_name = addonsJSON.contains("name") ? addonsJSON["name"].get<std::string>() : this->_id;
        this->_description = addonsJSON.contains("description") ? addonsJSON["description"].get<std::string>() : "NO DESCRIPTION PROVIDED";
        this->_logDecoration = addonsJSON.contains("log-decoration") ? addonsJSON["log-decoration"].get<std::string>() : "";

        auto jsonRequires = addonsJSON["requires"];
        if (jsonRequires.is_array())
        {
            for (auto it = jsonRequires.begin(); it != jsonRequires.end(); ++it)
            {
                if ((*it).is_string())
                    this->_requires.push_back((*it).get<std::string>());
            }
        }
        auto jsonConflicts = addonsJSON["conflicts"];
        if (jsonConflicts.is_array())
        {
            for (auto it = jsonConflicts.begin(); it != jsonConflicts.end(); ++it)
            {
                if ((*it).is_string())
                    this->_conflicts.push_back((*it).get<std::string>());
            }
        }
    }

    if (FindAddon(this->_id))
    {
        errorMessage = "Addon ID is already registered";
        goto label_error;
    }

    this->_isLoaded = true;
    api::GetSuitiumLogger()->Log("Loaded addon \"{}{}<reset>\"!", this->GetLogDecoration(), this->ID());

    this->_logger = std::make_unique<api::Logger>(this->GetName(), this->GetLogDecoration());

    return;

label_error: {}
    api::GetSuitiumLogger()->Log("<red>Could not load addon: {}.", errorMessage);
}
void Addon::Unload()
{
    this->_isLoaded = false;
    
    this->_requires.clear();
    this->_conflicts.clear();

    this->_logger.reset();

    this->_addonThread.reset();
}

void Addon::LoadAsSR()
{
    this->_id = "sub_rosa";

    this->_name = "Sub Rosa";
    this->_description = "The base Sub Rosa game.";
    this->_logDecoration = "<blue><b>";

    this->_requires.clear();
    this->_conflicts.clear();

    this->_logger.reset();

    this->_isLoaded = true;
}
void Addon::LoadAsSuitium()
{
    this->_id = "suitium";

    this->_name = "Suitium";
    this->_description = "Suitium reserved addon.";
    this->_logDecoration = "<red><b>";

    this->_requires.clear();
    this->_conflicts.clear();

    this->_logger.reset();

    this->_isLoaded = true;
}

const std::string &Addon::ID() const
{
    if (!this->IsLoaded())
        throw std::logic_error("Addon is not loaded");
    return this->_id;
}

const std::string &Addon::GetName() const
{
    if (!this->IsLoaded())
        throw std::logic_error("Addon is not loaded");
    return this->_name;
}
const std::string &Addon::GetDescription() const
{
    if (!this->IsLoaded())
        throw std::logic_error("Addon is not loaded");
    return this->_description;
}
const std::string &Addon::GetLogDecoration() const
{
    if (!this->IsLoaded())
        throw std::logic_error("Addon is not loaded");
    return this->_logDecoration;
}

bool Addon::CheckDependencies()
{
    if (!this->IsLoaded())
        throw std::logic_error("Addon is not loaded");
    if (this->_asType >= 0)
        return true;

    for (auto it = this->_requires.begin(); it != this->_requires.end(); ++it)
    {
        if (!FindAddon(*it))
        {
            api::GetSuitiumLogger()->Log("<red>\"{}\" addon was unloaded because required addon \"{}\" could not be found.", this->ID(), *it);
            this->Unload();
            return false;
        }
    }
    for (auto it = this->_conflicts.begin(); it != this->_conflicts.end(); ++it)
    {
        if (FindAddon(*it))
        {
            api::GetSuitiumLogger()->Log("<red>\"{}\" addon was unloaded because it conflicts with addon \"{}\".", this->ID(), *it);
            this->Unload();
            return false;
        }
    }

    return true;
}

api::Logger *Addon::GetLogger() const
{
    if (!this->IsLoaded())
        throw std::logic_error("Addon is not loaded");
    if (this->_asType >= 0)
    {
        if (this->_asType == 0)
            return api::GetSRLogger();
        else if (this->_asType == 1)
            return api::GetSuitiumLogger();
    }
    return this->_logger.get();
}

bool Addon::PrepareLua(LuaManager *manager, bool ignoreClient)
{
    if (!this->IsLoaded())
        throw std::logic_error("Addon is not loaded");
    if (this->_asType >= 0)
        return true;

    this->_addonThread = std::make_unique<sol::thread>(manager->L()->lua_state());
    manager->SetCurrentAddon(this);

    {
        std::string path = "/server/init.lua";
        std::string fullPath = this->_folderPath + path;
        std::ifstream stream(fullPath);
        if (stream.good())
        {
            std::stringstream contentStream;
            contentStream << stream.rdbuf();
            std::string content = contentStream.str();

            sol::load_result loadResult = this->_addonThread->state().load(content);
            if (loadResult.valid())
            {
                auto protectedFunc = loadResult.get<sol::protected_function>();
                sol::protected_function_result callResult = protectedFunc();
                if (!callResult.valid())
                {
                    api::GetSuitiumLogger()->Log("<red>\"{}\" addon was unloaded because an error occured while trying to run lua file \"{}\":\n{}", this->ID(), path, callResult.get<std::string>());
                    this->Unload();
                    return false;
                }
            }
            else
            {
                api::GetSuitiumLogger()->Log("<red>\"{}\" addon was unloaded because an error occured while trying to load lua file \"{}\":\n{}", this->ID(), path, loadResult.get<std::string>());
                this->Unload();
                return false;
            }
        }
    }


    if (ignoreClient)
        return true;

    {
        std::string path = "/client/init.lua";
        std::string fullPath = this->_folderPath + path;
        std::ifstream stream(fullPath);
        if (stream.good())
        {
            std::stringstream contentStream;
            contentStream << stream.rdbuf();
            std::string content = contentStream.str();

            sol::load_result loadResult = this->_addonThread->state().load(content);
            if (loadResult.valid())
            {
                auto protectedFunc = loadResult.get<sol::protected_function>();
                sol::protected_function_result callResult = protectedFunc();
                if (!callResult.valid())
                {
                    api::GetSuitiumLogger()->Log("<red>\"{}\" addon was unloaded because an error occured while trying to run lua file \"{}\": {}", this->ID(), path, callResult.get<std::string>());
                    this->Unload();
                    return false;
                }
            }
            else
            {
                api::GetSuitiumLogger()->Log("<red>\"{}\" addon was unloaded because an error occured while trying to load lua file \"{}\": {}", this->ID(), path, loadResult.get<std::string>());
                this->Unload();
                return false;
            }
        }
    }

    return true;
}

void DiscoverAddons()
{
    addons.clear();

    const std::string addonsFolderPath = "suitium/addons";

    {
        std::string srAddonPath = addonsFolderPath + "/sub_rosa";
        addons.push_back(std::make_unique<Addon>(srAddonPath, 0));
    }
    {
        std::string suitiumAddonPath = addonsFolderPath + "/suitium";
        addons.push_back(std::make_unique<Addon>(suitiumAddonPath, 1));
    }

    for (auto &entry : std::filesystem::directory_iterator(addonsFolderPath))
    {
        if (std::filesystem::is_directory(entry.status()))
        {
            if (entry.path().filename() == "sub_rosa")
            {
                api::GetSuitiumLogger()->Log("<yellow>Addon folder found using reserved name \"sub_rosa\", ignored.");
                continue;
            }
            if (!std::ifstream(entry.path().string() + "/addon.json").good())
            {
                api::GetSuitiumLogger()->Log("<yellow>Addon folder did not contain addon.json file, ignored.");
                continue;
            }
            addons.push_back(std::make_unique<Addon>(entry.path().string()));
        }
    }
}
const std::vector<std::unique_ptr<Addon>> &GetAddons()
{
    return addons;
}
Addon *FindAddon(const std::string &addonID)
{
    for (auto it = addons.begin(); it != addons.end(); ++it)
    {
        if ((*it)->IsLoaded() && (*it)->ID() == addonID)
            return (*it).get();
    }
    return nullptr;
}