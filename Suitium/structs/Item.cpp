#include "Item.hpp"

#include "../Addresses.hpp"
#include "ItemType.hpp"

namespace structs
{
	ItemType *Item::GetType() const
	{
		return &addresses::ItemTypes[this->typeID];
	}
}