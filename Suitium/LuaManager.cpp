#include "LuaManager.hpp"

#include <memory>
#include <sol/sol.hpp>
#include <sstream>
#include <string>
#include <stdexcept>
#include <subhook.h>
#include <utility>

#include "Addon.hpp"
#include "Addresses.hpp"
#include "api/Logging.hpp"
#include "hooks/CreateItem.hpp"
#include "hooks/CreateVehicle.hpp"
#include "structs/Item.hpp"
#include "structs/ItemType.hpp"
#include "structs/Vehicle.hpp"
#include "structs/VehicleType.hpp"
#include "TypeManager.hpp"

// https://github.com/moonjit/moonjit/blob/master/doc/c_api.md#luajit_setmodel-idx-luajit_mode_wrapcfuncflag
static int wrapExceptions(lua_State* L, lua_CFunction f) 
{
	try
    {
		return f(L);
	} catch (const char* s)
    {
		lua_pushstring(L, s);
	} catch (const std::exception& e)
    {
		lua_pushstring(L, e.what());
	} catch (...)
    {
		lua_pushliteral(L, "caught (...)");
	}
	return lua_error(L);
}

LuaManager::LuaManager()
{

}

void LuaManager::Initialize()
{
    this->_L = std::make_unique<sol::state>();

    lua_pushlightuserdata(*this->_L, (void*)wrapExceptions);
	luaJIT_setmode(*this->_L, -1, LUAJIT_MODE_WRAPCFUNC | LUAJIT_MODE_ON);
	lua_pop(*this->_L, 1);

    this->_L->open_libraries(sol::lib::base);
	//this->_L->open_libraries(sol::lib::package);
	//this->_L->open_libraries(sol::lib::coroutine);
	this->_L->open_libraries(sol::lib::string);
	this->_L->open_libraries(sol::lib::math);
	this->_L->open_libraries(sol::lib::table);
	this->_L->open_libraries(sol::lib::bit32);

	(*this->_L)["print"] = [&](sol::this_state &state, sol::variadic_args &args)
	{
		std::stringstream stream;
		for (std::string str : args)
			stream << str << '\t';

		std::string text = stream.str();
		text.pop_back(); // Pop last tab
		this->GetCurrentAddon()->GetLogger()->LogText(text);
	};
	(*this->_L)["warn"] = [&](sol::this_state &state, sol::variadic_args &args)
	{
		std::stringstream stream;
		for (std::string str : args)
			stream << str << '\t';

		std::string text = stream.str();
		text.pop_back(); // Pop last tab
		this->GetCurrentAddon()->GetLogger()->LogText(std::string("<yellow>") + text);
	};

	this->_L->new_usertype<LuaHook>(
		"Hook",

		sol::call_constructor, [&](std::string &hookName, sol::function &hookFunction, sol::variadic_args flags)
		{
			std::shared_ptr<LuaHook> hook = std::make_shared<LuaHook>();

			hook->active = true;
			hook->addon = this->GetCurrentAddon();
			hook->name = hookName;
			hook->function = (sol::protected_function)hookFunction;
			if (flags.size() > 0)
			{
				for (std::string flag : flags)
					hook->flags.push_back(flag);
			}
			else
				hook->flags = std::vector<std::string>{ "pre" };

			this->_hooks.push_back(hook);

			return hook;
		},

		"Remove", &LuaHook::Remove
	);

	this->_L->new_usertype<Addon>(
		"Addon",
		sol::no_constructor,

		"isLoaded", sol::property(&Addon::IsLoaded),

		"id", sol::property(&Addon::ID),

		"name", sol::property(&Addon::GetName),
		"description", sol::property(&Addon::GetDescription),
		"logDecoration", sol::property(&Addon::GetLogDecoration),

		"GetAll", [&](sol::this_state &state)
		{
			sol::table table = sol::table::create(state.L);
			std::size_t sz = 1;
			for (const std::unique_ptr<Addon> &addon : GetAddons())
			{
				table[sz] = addon.get();
				sz++;
			}
			return table;
		},
		"Current", sol::property([&]()
		{
			return this->GetCurrentAddon();
		})
	);
	
	this->DefineGameTypes();
}
void LuaManager::Deinitialize()
{
	this->_hooks.clear();

    this->_L.reset();
}
void LuaManager::DefineGameTypes()
{
	this->_L->new_usertype<structs::CInteger>(
		"Integer",
		sol::no_constructor,

		"value", &structs::CInteger::i,

		"__tostring", &structs::CInteger::operator std::string
	);

	this->_L->new_usertype<structs::CBoolean>(
		"Boolean",
		sol::no_constructor,
		
		"value", &structs::CBoolean::b1,

		"__tostring", &structs::CBoolean::operator std::string
	);

	this->_L->new_usertype<structs::CVector3>(
		"Vector3",

		"x", &structs::CVector3::x,
		"y", &structs::CVector3::y,
		"z", &structs::CVector3::z,

		sol::call_constructor, [](float n)
		{
			return (structs::CVector3)glm::vec3(n);
		},
		sol::call_constructor, [](float x, float y, float z)
		{
			return (structs::CVector3)glm::vec3(x, y, z);
		},

		"Dot", &structs::CVector3::Dot,
		"Cross", &structs::CVector3::Cross,

		"Set", &structs::CVector3::Set,

		"__tostring", &structs::CVector3::operator std::string,

		"Zero", sol::property([]()
		{
			return (structs::CVector3)glm::vec3(0.0f);
		}),
		"One", sol::property([]()
		{
			return (structs::CVector3)glm::vec3(1.0f);
		})
	);

	this->_L->new_usertype<structs::COrientation>(
		"Orientation",

		"right", &structs::COrientation::right,
		"up",    &structs::COrientation::up,
		"back",  &structs::COrientation::back,

		sol::call_constructor, []()
		{
			return (structs::COrientation)glm::mat3(1.0f);
		},

		"Set", &structs::COrientation::Set,

		"__tostring", &structs::COrientation::operator std::string,

		"Identity", sol::property([]()
		{
			return (structs::COrientation)glm::mat3(1.0f);
		})
	);

	this->_L->new_usertype<structs::ItemType>(
		"ItemType",
		sol::no_constructor,

		"name", sol::property(&structs::ItemType::GetName, &structs::ItemType::SetName),

		"index", sol::property(&structs::ItemType::GetIndex),
		"typeID", sol::property(&structs::ItemType::GetTypeID),

		"GetAll", [](sol::this_state &state)
		{
			sol::table table = sol::table::create(state.L);
			for (std::size_t itemTypeCount = 0; itemTypeCount < structs::ItemType::VanillaCount; itemTypeCount++)
				table[itemTypeCount + 1] = &addresses::ItemTypes[itemTypeCount];
			return table;
		},
		"GetByID", [](const std::string &typeID)
		{
			std::pair<std::string, std::string> decomposed = DecomposeTypeID(typeID);
			return &addresses::ItemTypes[GetItemTypeManager()->GetID(decomposed.first, decomposed.second)];
		}
	);

	this->_L->new_usertype<structs::Item>(
		"Item",
		sol::no_constructor,

		"isActive", &structs::Item::isActive,
		"type", sol::property(&structs::Item::GetType),

		"position", &structs::Item::position,
		"velocity", &structs::Item::velocity,
		"orientation", &structs::Item::orientation,

		"Create", sol::overload(
			[](structs::ItemType &typeID, structs::CVector3 &position, structs::COrientation &orientation)
			{
				structs::CVector3 velocity = glm::vec3(0.0f);

				subhook::ScopedHookRemove scopedRemove(createItemHook);
				int itemID = addresses::CreateItemFunc(typeID.customData.index, &position, &velocity, &orientation);
				if (itemID < 0)
					throw std::runtime_error("Could not create item");
				return &addresses::Items[itemID];
			},
			[](structs::ItemType &typeID, structs::CVector3 &position, structs::COrientation &orientation, structs::CVector3 &velocity)
			{
				subhook::ScopedHookRemove scopedRemove(createItemHook);
				int itemID = addresses::CreateItemFunc(typeID.customData.index, &position, &velocity, &orientation);
				if (itemID < 0)
					throw std::runtime_error("Could not create item");
				return &addresses::Items[itemID];
			})
	);

	this->_L->new_usertype<structs::VehicleType>(
		"VehicleType",
		sol::no_constructor,

		"name", sol::property(&structs::VehicleType::GetName, &structs::VehicleType::SetName),
		"price", &structs::VehicleType::price,
		"mass", &structs::VehicleType::mass,

		"index", sol::property(&structs::VehicleType::GetIndex),
		"typeID", sol::property(&structs::VehicleType::GetTypeID),

		"GetAll", [](sol::this_state &state)
		{
			sol::table table = sol::table::create(state.L);
			for (std::size_t vehicleTypeCount = 0; vehicleTypeCount < structs::VehicleType::VanillaCount; vehicleTypeCount++)
				table[vehicleTypeCount + 1] = &addresses::VehicleTypes[vehicleTypeCount];
			return table;
		},
		"GetByID", [](const std::string &typeID)
		{
			std::pair<std::string, std::string> decomposed = DecomposeTypeID(typeID);
			return &addresses::VehicleTypes[GetVehicleTypeManager()->GetID(decomposed.first, decomposed.second)];
		}
	);

	this->_L->new_usertype<structs::Vehicle>(
		"Vehicle",
		sol::no_constructor,

		"isActive", &structs::Vehicle::isActive,
		"type", sol::property(&structs::Vehicle::GetType),

		"position", &structs::Vehicle::position,
		"velocity", &structs::Vehicle::velocity,
		"orientation", &structs::Vehicle::orientation,

		"Create", sol::overload(
			[](structs::VehicleType &typeID, int colorID, structs::CVector3 &position, structs::COrientation &orientation)
			{
				structs::CVector3 velocity = glm::vec3(0.0f);

				subhook::ScopedHookRemove scopedRemove(createVehicleHook);
				int vehicleID = addresses::CreateVehicleFunc(typeID.customData.index, &position, &velocity, &orientation, colorID);
				if (vehicleID < 0)
					throw std::runtime_error("Could not create vehicle");
				return &addresses::Vehicles[vehicleID];
			},
			[](structs::VehicleType &typeID, int colorID, structs::CVector3 &position, structs::COrientation &orientation, structs::CVector3 &velocity)
			{
				subhook::ScopedHookRemove scopedRemove(createVehicleHook);
				int vehicleID = addresses::CreateVehicleFunc(typeID.customData.index, &position, &velocity, &orientation, colorID);
				if (vehicleID < 0)
					throw std::runtime_error("Could not create vehicle");
				return &addresses::Vehicles[vehicleID];
			})
	);
}

Addon *LuaManager::GetCurrentAddon()
{
	lua_State *L = this->_L->lua_state();
	lua_rawgeti(L, LUA_REGISTRYINDEX, LUAMANAGER_LUAADDONINDEX);
	void *ptr = lua_touserdata(L, -1);
	lua_pop(L, 1);
	return (Addon *)ptr;
}
void LuaManager::SetCurrentAddon(Addon *addon)
{
	lua_State *L = this->_L->lua_state();
	lua_pushlightuserdata(L, addon);
	lua_rawseti(L, LUA_REGISTRYINDEX, LUAMANAGER_LUAADDONINDEX);
}

const std::vector<std::shared_ptr<LuaHook>> &LuaManager::GetHooks() const
{
	return this->_hooks;
}
void LuaManager::CheckHooks()
{
	for (auto it = this->_hooks.begin(); it != this->_hooks.end(); ++it)
	{
		if (!(*it)->active)
		{
			this->_hooks.erase(it);
			break;
		}
	}
}

sol::state *LuaManager::L() const
{
    return this->_L.get();
}

LuaManager *GetMainLuaManager()
{
    static LuaManager s;
    return &s;
}