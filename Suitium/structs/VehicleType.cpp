#include "VehicleType.hpp"

#include <string>

namespace structs
{
    std::string VehicleType::GetName() const
    {
        return std::string(this->name);
    }
    void VehicleType::SetName(const std::string &right)
    {
        right.copy(this->name, sizeof(this->name));
    }

    int VehicleType::GetIndex() const
    {
        return this->customData.index;
    }
    std::string VehicleType::GetTypeID() const
    {
        return *this->customData.typeIDPtr;
    }
}