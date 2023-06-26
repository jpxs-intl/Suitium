#pragma once

#include <cmath>
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <string>

namespace structs
{
#if _WIN32
#pragma pack(push, 1)
#endif
    struct CPad
    {
        char p;
#if __linux__
    } __attribute__((packed));
#elif _WIN32
    };
#pragma pack(pop)
#endif

#if _WIN32
#pragma pack(push, 1)
#endif
    struct CInteger
    {
        int i;

        CInteger()
        {
            this->i = 0;
        }
        CInteger(const CInteger &right)
        {
            this->i = right.i;
        }
        CInteger(int integer)
        {
            this->i = integer;
        }

        operator int() const
        {
            return this->i;
        }
        CInteger &operator=(int right)
        {
            this->i = right;
            return *this;
        }

        operator std::string() const
        {
            return std::to_string(this->i);
        }
#if __linux__
    } __attribute__((packed));
#elif _WIN32
    };
#pragma pack(pop)
#endif

#if _WIN32
#pragma pack(push, 1)
#endif
    struct CBoolean
    {
        struct
        {
            bool b1;
            bool b2;
            bool b3;
            bool b4;
#if __linux__
        } __attribute__((packed));
#else
        };
#endif

        CBoolean()
        {
            this->b1 = false;
            this->b2 = false;
            this->b3 = false;
            this->b4 = false;
        }
        CBoolean(const CBoolean &right)
        {
            this->b1 = right.b1;
            this->b2 = false;
            this->b3 = false;
            this->b4 = false;
        }
        CBoolean(bool boolean)
        {
            this->b1 = boolean;
            this->b2 = false;
            this->b3 = false;
            this->b4 = false;
        }
        
        operator bool() const
        {
            return this->b1;
        }
        CBoolean &operator=(bool right)
        {
            this->b1 = right;
            return *this;
        }

        operator std::string() const
        {
            return this->b1 ? "true" : "false";
        }
#if __linux__
    } __attribute__((packed));
#elif _WIN32
    };
#pragma pack(pop)
#endif

#if _WIN32
#pragma pack(push, 1)
#endif
    struct CVector3
    {
        float x;
        float y;
        float z;

        CVector3()
        {
            this->x = 0.0f;
            this->y = 0.0f;
            this->z = 0.0f;
        }
        CVector3(const CVector3 &other)
        {
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
        }
        CVector3(float x, float y, float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        CVector3 &operator =(const CVector3 &right)
        {
            this->x = right.x;
            this->y = right.y;
            this->z = right.z;
            return *this;
        }

        float Length() const
        {
            return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
        }
        float Dot(const CVector3 &right) const
        {
            return this->x * right.x + this->y * right.y + this->z * right.z;
        }
        CVector3 Cross(const CVector3 &right) const
        {
            glm::vec3 result = glm::cross(glm::vec3(this->x, this->y, this->z), glm::vec3(right.x, right.y, right.z));
            return CVector3(result.x, result.y, result.z);
        }

        operator std::string() const
        {
            return fmt::format("{}, {}, {}", this->x, this->y, this->z);
        }
#if __linux__
    } __attribute__((packed));
#elif _WIN32
    };
#pragma pack(pop)
#endif

#if _WIN32
#pragma pack(push, 1)
#endif
    struct COrientation
    {
        CVector3 right;
        CVector3 up;
        CVector3 back;

        COrientation()
        {
            this->right = CVector3(1.0f, 0.0f, 0.0f);
            this->up = CVector3(0.0f, 1.0f, 0.0f);
            this->back = CVector3(0.0f, 0.0f, 1.0f);
        }
        COrientation(const COrientation &other)
        {
            this->right = right;
            this->up = up;
            this->back = back;
        }

        COrientation &operator =(const COrientation &right)
        {
            this->right = right.right;
            this->up = right.up;
            this->back = right.back;
            return *this;
        }

        operator std::string() const
        {
            return fmt::format("{},\n{},\n{}", (std::string)this->right, (std::string)this->up, (std::string)this->back);
        }
#if __linux__
    } __attribute__((packed));
#elif _WIN32
    };
#pragma pack(pop)
#endif
}