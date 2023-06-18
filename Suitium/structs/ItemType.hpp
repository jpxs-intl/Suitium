#pragma once

#include <cstddef>
#include <cstdint>

#include "Common.hpp"

namespace structs
{
    //  5072 (0x13D0)
    struct ItemType
    {
        static constexpr std::size_t VanillaCount = 46;

        struct CustomData
        {
            int index;
            std::string *typeIDPtr;
        };

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
        CustomData customData;
        CPad pad10[4544 - sizeof(CustomData) - 4]; // For some reason it crashes if I don't add some extra space
        CVector3 gunHoldPosition;
        CPad pad11[48];

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