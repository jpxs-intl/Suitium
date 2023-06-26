#include "Text.hpp"

#include "../Addresses.hpp"

namespace api
{
    void DrawText(const std::string &text, float x, float y, float size, const glm::vec4 &color, TextAlignment alignment, bool shadows)
    {
        std::string safeText = text; // We need to replace nulls with another valid character
        for (auto it = safeText.begin(); it != safeText.end(); ++it)
        {
            if (*it == '\0')
                *it = '?';
        }

        unsigned int flags = 0x40;
        if (alignment != TextAlignment::Right)
            flags |= (1 << ((unsigned int)alignment - 1));

#if _WIN32
        addresses::CSDrawTextFunc(safeText.c_str(), x, y, size, flags | 0x40, color[0], color[1], color[2], color[3]);
#elif __linux__
        addresses::CSDrawTextFunc(safeText.c_str(), flags | 0x40, 0, 0, x, y, size, color[0], color[1], color[2], color[3], nullptr);
#endif
    }
}