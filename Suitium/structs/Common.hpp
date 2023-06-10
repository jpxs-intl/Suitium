#pragma once

#include <glm/glm.hpp>

namespace structs
{
    struct CPad
    {
        char p;
    };

    struct CBoolean
    {
        int i;
        
        operator bool() const
        {
            return (bool)this->i;
        }
        CBoolean &operator=(bool right)
        {
            this->i = (int)right;
            return *this;
        }
    };

    struct CVector3
    {
        float a[3];

        operator glm::vec3() const
        {
            return glm::vec3(this->a[0], this->a[1], this->a[2]);
        }
        CVector3 &operator=(const glm::vec3 &right)
        {
            this->a[0] = right.x;
            this->a[1] = right.y;
            this->a[2] = right.z;
            return *this;
        }
    };

    struct COrientation
    {
        CVector3 a[3];

        operator glm::mat3() const
        {
            return glm::mat3((glm::vec3)this->a[0], (glm::vec3)this->a[1], (glm::vec3)this->a[2]);
        }
        COrientation &operator=(const glm::mat3 &right)
        {
            this->a[0] = right[0];
            this->a[1] = right[1];
            this->a[2] = right[2];
            return *this;
        }
    };
}