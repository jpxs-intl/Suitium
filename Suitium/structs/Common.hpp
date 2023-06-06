#pragma once

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
        bool operator=(bool right)
        {
            this->i = (int)right;
            return this->operator bool();
        }
    };

    struct CVector3
    {
        float x, y, z;
    };

    struct COrientation
    {
        CVector3 right, up, back;
    };
}