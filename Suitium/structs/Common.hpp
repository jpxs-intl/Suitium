#pragma once

#include <fmt/format.h>
#include <glm/glm.hpp>
#include <string>

namespace structs
{
    struct CPad
    {
        char p;
    };

    struct CBoolean
    {
        union
        {
            int i;
            struct
            {
                bool b1;
                bool b2;
                bool b3;
                bool b4;
            }; // i love little endian
        };
        
        operator bool() const
        {
            return (bool)this->i;
        }
        CBoolean &operator=(bool right)
        {
            this->i = (int)right;
            return *this;
        }

        operator std::string() const
        {
            return this->b1 ? "true" : "false";
        }
    };

    struct CVector3
    {
        union
        {
            glm::vec3 v;
            struct
            {
                float x;
                float y;
                float z;
            };
        };

        CVector3()
        {
            this->v = glm::vec3(0.0f);
        }
        CVector3(const CVector3 &right)
        {
            this->v = right.v;
        }
        CVector3(const glm::vec3 &vec)
        {
            this->v = vec;
        }

        operator glm::vec3() const
        {
            return this->v;
        }
        CVector3 &operator=(const glm::vec3 &right)
        {
            this->v = right;
            return *this;
        }

        float Dot(const CVector3 &right) const
        {
            return glm::dot(this->v, right.v);
        }
        CVector3 Cross(const CVector3 &right) const
        {
            return glm::cross(this->v, right.v);
        }

        void Set(const CVector3 &right)
        {
            *this = right;
        }

        operator std::string() const
        {
            return fmt::format("{}, {}, {}", this->x, this->y, this->z);
        }
    };

    struct COrientation
    {
        union
        {
            glm::mat3 v;
            struct
            {
                CVector3 right;
                CVector3 up;
                CVector3 back;
            };
        };

        COrientation()
        {
            this->v = glm::mat3(1.0f);
        }
        COrientation(const COrientation &right)
        {
            this->v = right;
        }
        COrientation(const glm::mat3 &mat)
        {
            this->v = mat;
        }

        operator glm::mat3() const
        {
            return this->v;
        }
        COrientation &operator=(const glm::mat3 &right)
        {
            this->v = right;
        }

        void Set(const COrientation &right)
        {
            *this = right;
        }

        operator std::string() const
        {
            return fmt::format("{},\n{},\n{}", (std::string)this->right, (std::string)this->up, (std::string)this->back);
        }
    };
}