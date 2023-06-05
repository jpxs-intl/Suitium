#pragma once

#include <cstddef>

#include "Common.hpp"

struct VehicleType
{
    static constexpr std::size_t VanillaCount = 17;

    int usesExternalModel;
    CBoolean useUnstablePhysics; // Only available on Win32
    int controllerType;
    CPad pad2[8];
    char name[32];
    int price;
    float mass;
    CPad pad3[99716];
};