#include "ItemType.hpp"

#include <string>

namespace structs
{
    std::string ItemType::GetName() const
    {
        return std::string(this->name);
    }
    void ItemType::SetName(const std::string &right)
    {
        right.copy(this->name, sizeof(this->name));
    }

    int ItemType::GetIndex() const
    {
        return this->customData.index;
    }
    std::string ItemType::GetTypeID() const
    {
        return *this->customData.typeIDPtr;
    }
}