#pragma once

#include <cstddef>
#include <cstdint>

#include "Common.hpp"

namespace structs
{
#if _WIN32
#pragma pack(push, 1)
#endif
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
        CPad pad10[4544 - sizeof(CustomData)];
        CVector3 gunHoldPosition;
        CPad pad11[48];

        std::string GetName() const;
        void SetName(const std::string &right);

        int GetIndex() const;
        std::string GetTypeID() const;
#if __linux__
    } __attribute__((packed));
#else
    };
#pragma pack(pop)
#endif
}