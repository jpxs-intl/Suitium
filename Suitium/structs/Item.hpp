#pragma once

#include <cstddef>

#include "Common.hpp"

namespace structs
{
    // 7040 (0x1B80)
    struct Item
    {
        static constexpr std::size_t VanillaCount = 1024;

        CBoolean isActive;
        CBoolean isPhysicsEnabled;
        CBoolean isPhysicsSettled;
        int physicsSettledTimer;
        CBoolean isStatic;
        int type;
        CPad pad1[4];
        int despawnTimer;
        int grenadePrimerID;
        int parentHumanID;
        int parentItemID;
        int parentInventorySlotID;
        CPad pad2[40];
        int rigidBodyID;
        CVector3 position;
        CVector3 alternativePosition;
        CVector3 velocity;
        CVector3 alternativeVelocities[3];
        COrientation orientation;
        CPad pad3[116];
        int cooldown;
        CPad pad4[4];
        int ammoCount;
        CPad pad5[20];
        int connectedPhoneItemID;
        int phoneNumber;
        CPad pad6[4];
        int displayPhoneNumber;
        int enteredPhoneNumber;
        CPad pad7[264];
        int phoneTextureID;
        CPad pad8[4];
        int keyVehicleID;
        CPad pad9[28];
        int cashSpread;
        int cashBillAmount;
        int cashPureValue;
        CPad pad10[188];
        unsigned int computerCurrentLine;
        unsigned int computerTopLine;
        int computerCursor; // -1 is no cursor
        char computerChars[32][64];
        unsigned char computerColors[32][64];
        CPad pad11[740];
        int computerTeamID;
        CPad pad12[1316];
    };
}