#pragma once

#include <cstddef>

#include "Common.hpp"

namespace structs
{
#if _WIN32
#pragma pack(push, 1)
#endif
    // 99776 (0x185C0)
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
        CPad pad2[99716 - sizeof(CustomData)];
        CustomData customData;

        std::string GetName() const;
        void SetName(const std::string &right);

        int GetIndex() const;
        std::string GetTypeID() const;
#if __linux__
    } __attribute__((packed));
#elif _WIN32
    };
#pragma pack(pop)
#endif
}