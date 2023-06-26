#pragma once

#include <cstddef>
#include <glad/glad.hpp>

#ifndef GLuint
typedef unsigned int GLuint;
#endif

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
        CPad pad1[24];
        GLuint glTexture;
        CPad pad2[49130];
#if __linux__
    } __attribute__((packed));
#elif _WIN32
    };
#pragma pack(pop)
#endif
}