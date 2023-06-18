#pragma once

#include <cstddef>

#include "Common.hpp"

namespace structs
{
    struct VehicleType
    {
        static constexpr std::size_t VanillaCount = 17;

        struct CustomData
        {
            int health;

            int index;
            std::string *typeIDPtr;
        };

        CBoolean usesExternalModel;
        CBoolean useUnstablePhysics;
        int controllerTypeID;
        CPad pad1[8];
        char name[32];
        int price;
        float mass;
        CPad pad2[99716 - sizeof(CustomData) - 4];  // For some reason it crashes if I don't add some extra space
        CustomData customData;

        std::string GetName() const
        {
            return std::string(this->name);
        }
        void SetName(const std::string &right)
        {
            right.copy(this->name, sizeof(this->name));
        }

        int GetIndex() const
        {
            return this->customData.index;
        }
        std::string GetTypeID() const
        {
            return *this->customData.typeIDPtr;
        }
    };
}