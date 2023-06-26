#pragma once

#include <glm/glm.hpp>
#include <string>

#if _WIN32
#undef DrawText
#endif

namespace api
{
    enum class TextAlignment
    {
        Right,
        Center,
        Left,
        Up,
        Down
    };

    void DrawText(const std::string &text, float x, float y, float size, const glm::vec4 &color, TextAlignment alignment = TextAlignment::Center, bool shadows = false);
}