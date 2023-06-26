#include "Vehicle.hpp"

#include "../Addresses.hpp"
#include "VehicleType.hpp"

namespace structs
{
	VehicleType *Vehicle::GetType() const
	{
		return &addresses::VehicleTypes[this->typeID];
	}
}