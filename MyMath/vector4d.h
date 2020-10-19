#ifndef VECTOR4D_H
#define VECTOR4D_H

#include "MyMath/vector3d.h"

class Vector4D
{
    const unsigned int vectorSize = 4;
    Vector3D vector3d;
    float dimension4Value = 1.f;

public:
    Vector4D();
    Vector4D(Vector3D v, float d4v = 1.f);

    float getdimension4Value()  {
        return dimension4Value;
    }
    Vector3D getVector3D() {
        return vector3d;
    }

    Vector4D operator = (Vector4D v)    {
        vector3d = v.vector3d;
        dimension4Value = v.dimension4Value;

        return *this;
    }
};

#endif // VECTOR4D_H
