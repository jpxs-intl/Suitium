#include "TypeManager.hpp"

#include <array>
#include <stdexcept>
#include <string>
#include <utility>

TypeManager::TypeManager()
{
	this->_currentID = 0;
}
TypeManager::~TypeManager()
{
}

bool TypeManager::HasID(const std::string &addonID, const std::string &entryName) const
{
	if (this->_registry.count(addonID) == 0 || this->_registry.at(addonID).count(entryName) == 0)
		return false;
	return true;
}
std::size_t TypeManager::GetID(const std::string &addonID, const std::string &entryName) const
{
	if (!this->HasID(addonID, entryName))
		throw std::logic_error("Entry does not exist");
	return this->_registry.at(addonID).at(entryName);
}
std::size_t TypeManager::NewID(const std::string &addonID, const std::string &entryName)
{
	if (this->HasID(addonID, entryName))
		throw std::logic_error("Entry was already registered");

	this->_currentID++;

	this->_registry[addonID][entryName] = this->_currentID - 1;
	return this->_currentID - 1;
}

void TypeManager::Clear()
{
	this->_registry.clear();
	this->_currentID = 0;
}

bool IsTypeIDValid(const std::string &typeID)
{
	static const std::array<std::pair<char, int>, 5> matches = {
		std::make_pair(':', 1),

		std::make_pair(' ', 0),
		std::make_pair('-', 0),
		std::make_pair('+', 0),
		std::make_pair('.', 0)
	};

	for (std::pair<char, int> match : matches)
	{
		if (std::count(typeID.begin(), typeID.end(), match.first) != match.second)
			return false;
	}

	for (auto it = typeID.begin(); it != typeID.end(); ++it)
	{
		if (*it == ':')
		{
			if (it == typeID.begin() || it + 1 == typeID.end())
				return false;
		}
	}

	return true;
}
std::pair<std::string, std::string> DecomposeTypeID(const std::string &typeID)
{
	if (!IsTypeIDValid(typeID))
		throw std::logic_error("Invalid type ID");

	std::size_t separatorCount = 0;
	for (char c : typeID)
	{
		if (c == ':')
			break;
		separatorCount++;
	}

	return std::make_pair(typeID.substr(0, separatorCount), typeID.substr(separatorCount + 1, typeID.size() - separatorCount - 1));
}

TypeManager *GetItemTypeManager()
{
	static TypeManager s;
	return &s;
}
TypeManager *GetVehicleTypeManager()
{
	static TypeManager s;
	return &s;
}