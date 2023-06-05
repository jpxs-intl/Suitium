#pragma once

#include <cstddef>

#include "Common.hpp"

struct ItemType
{
    static constexpr std::size_t VanillaCount = 46;

    CPad pad1[4];
    int price;
    float mass;
    CPad pad2[4];
    CBoolean isGun;
    CBoolean useAlternativeAim;
    int fireRate;
    int bulletTypeID;
    CPad pad3[4];
    int ammoCount;
    float bulletVelocity;
    float bulletSpread;
    char name[64];
    CPad pad4[12];
    int handCount;
    CVector3 rightHandOffset;
    CVector3 leftHandOffset;
    CPad pad5[24];
    float primaryGripStiffness;
    CPad pad6[8];
    float primaryGripRotation;
    float secondaryGripStiffness;
    CPad pad7[8];
    float secondaryGripRotation;
    CPad pad8[52];
    CVector3 boundsCenter;
    CPad pad9[12];
    CBoolean linkableMap[46];
    CPad pad10[4544];
    CVector3 gunHoldPosition;
    CPad pad11[48];
};