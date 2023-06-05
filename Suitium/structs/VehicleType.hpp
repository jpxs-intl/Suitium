#pragma once

#include <cstddef>

#include "Common.hpp"

struct VehicleType
{
    static constexpr std::size_t VanillaCount = 17;

    CBoolean usesExternalModel;
    CBoolean useUnstablePhysics; // Only available on Win32 client? (I'm not actually sure)
    int controllerType;
    CPad pad2[8];
    char name[32];
    int price;
    float mass;
    CPad pad3[99716];
};