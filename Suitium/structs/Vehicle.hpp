#pragma once

#include <cstddef>

#include "Common.hpp"
#include "VehicleType.hpp"

namespace structs
{
    // 20840 (0x5168)
    struct Vehicle
    {
        static constexpr std::size_t VanillaCount = 512;

        CBoolean isActive;
        int typeID;
        int controllerTypeID;
        int health;
        CPad pad1[4];
        int lastDriverPlayerID;
        int colorID;
        short despawnTimer;
        short spawnedStateID;
        CBoolean isLocked;
        CPad pad3[4];
        int rigidBodyID;
        CVector3 position;
        CVector3 alternativePosition;
        COrientation orientation;
        CPad pad4[4];
        CVector3 velocity;
        CPad pad5[10116];
        CBoolean windowBrokenMap[8];
        CPad pad6[3556];
        float gearX;
        float steerControl;
        float gearY;
        float gasControl;
        CPad pad7[56];
        int trafficCarID;
        CPad pad8[740];
        int engineRPM;
        CPad pad9[5748];
        int bladeRigidBodyID;
        CPad pad10[304];
        int numberOfSeats;
        CPad pad11[136];

        VehicleType *GetType() const;
    };
}