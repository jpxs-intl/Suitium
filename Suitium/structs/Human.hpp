#pragma once

#include <cstddef>

#include "Common.hpp"

namespace structs
{
#if _WIN32
#pragma pack(push, 1)
#endif
    struct Bone
    {
        int rigidBodyID;
        CVector3 position;
        CVector3 alternativePosition;
        CVector3 velocity;
        CPad pad1[12];
        COrientation orientation;
        CPad pad2[224];
#if __linux__
    } __attribute__((packed));
#else
    };
#pragma pack(pop)
#endif

#if _WIN32
#pragma pack(push, 1)
#endif
    struct InventorySlot
    {
        int numberOfPlaces;
        int firstPlaceItemID;
        int secondPlaceItemID;
        CPad pad1[28];
#if __linux__
    } __attribute__((packed));
#else
    };
#pragma pack(pop)
#endif

#if _WIN32
#pragma pack(push, 1)
#endif
    struct Human
    {
        static constexpr std::size_t VanillaCount = 256;

        CBoolean isActive;
        CBoolean isPhysicsEnabled;
        int playerID;
        int accountID;
        CPad pad1[12];
        int stamina;
        int maximumStamina;
        CPad pad2[4];
        int vehicleID;
        int vehicleSeatID;
        int lastVehicleID;
        CPad pad3[4];
        int despawnTimer;
        CPad pad4[4];
        int isEliminator;
        CPad pad5[12];
        unsigned int spawnProtectionTimer;
        CBoolean isOnGround;
        int movementStateID;
        CPad pad6[4];
        int zoomLevel;
        CPad pad7[20];
        int damage;
        CBoolean isStanding;
        CVector3 position;
        CVector3 alternativePosition;
        float viewYaw;
        float viewPitch;
        CPad pad8[56];
        float alternativeViewYaw;
        CPad pad9[80];
        float rightInput;
        CPad pad10[4];
        float forwardInput;
        CPad pad11[4];
        float alternativeViewPitch;
        CPad pad12[212];
        unsigned int inputFlags;
        unsigned int previousInputFlags;
        CPad pad13[4];
        Bone bones[16];
        CPad pad14[21808];
        InventorySlot inventorySlots[6];
        CPad pad15[400];
        int health;
        int bloodLevel;
        CBoolean isBleeding;
        int torsoHealth;
        CPad pad16[4];
        int headHealth;
        CPad pad17[4];
        int leftArmHealth;
        CPad pad18[4];
        int rightArmHealth;
        CPad pad19[4];
        int leftLegHealth;
        CPad pad20[4];
        int rightLegHealth;
        CPad pad21[84];
        int progressBar;
        int inventoryAnimationFlags;
        float inventoryAnimationProgress;
        int inventoryAnimationDuration;
        int inventoryAnimationHand;
        int inventoryAnimationSlot;
        CBoolean inventoryAnimationCounterFinished;
        int inventoryAnimationCounter;
        CPad pad22[388];
        int genderID;
        int headModelID;
        int skinColorID;
        int hairColorID;
        int hairModelID;
        int eyeColorID;
        int modelID;
        int suitColorID;
        int tieColorID;
        CPad pad23[4];
        int necklaceID;
        int lastUpdatedWantedGroup;
        CPad pad24[72];
#if __linux__
    } __attribute__((packed));
#else
    };
#pragma pack(pop)
#endif
}