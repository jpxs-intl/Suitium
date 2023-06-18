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

        std::string GetName() const;
        void SetName(const std::string &right);

        int GetIndex() const;
        std::string GetTypeID() const;
    };
}