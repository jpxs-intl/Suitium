#pragma once

#include <cstddef>

#include "Common.hpp"

namespace structs
{
    struct VehicleType
    {
        static constexpr std::size_t VanillaCount = 17;

        CBoolean usesExternalModel;
        CBoolean useUnstablePhysics;
        int controllerTypeID;
        CPad pad1[8];
        char name[32];
        int price;
        float mass;
        CPad pad2[99716];
    };
}