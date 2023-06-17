#pragma once

#include <string>
#include <unordered_map>
#include <utility>

class TypeManager
{
public:
	TypeManager();
	TypeManager(const TypeManager &) = delete;
	~TypeManager();

private:
	std::unordered_map<std::string, std::unordered_map<std::string, std::size_t>> _registry;
	std::size_t _currentID;
public:
	bool HasID(const std::string &addonID, const std::string &entryName) const;
	std::size_t GetID(const std::string &addonID, const std::string &entryName) const;
	std::size_t NewID(const std::string &addonID, const std::string &entryName);
};

bool IsTypeIDValid(const std::string &typeID);
std::pair<std::string, std::string> DecomposeTypeID(const std::string &typeID);

TypeManager *GetItemTypeManager();