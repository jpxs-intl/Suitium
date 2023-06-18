#pragma once

#include <cstddef>

#include "Common.hpp"

namespace structs
{
#if _WIN32
#pragma pack(push, 1)
#endif
    struct CSTexture
    {
        int width;
        int height;
        CPad pad1[28];
        std::uint32_t glTexture;
        CPad pad2[49126];
#if __linux__
    } __attribute__((packed));
#else
    };
#pragma pack(pop)
#endif
}