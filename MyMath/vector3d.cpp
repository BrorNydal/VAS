#include "vector3d.h"

#include "mymathfunctions.h"

#include <math.h>

Vector3D::Vector3D()
{
    xyz[0]=0.f;
    xyz[1]=0.f;
    xyz[2]=0.f;
}

Vector3D::Vector3D(float X, float Y, float Z)
{
    xyz[0] = X;
    xyz[1] = Y;
    xyz[2] = Z;
}

Vector3D Vector3D::calculateNormal(Vector3D v1, Vector3D v2)
{
    return (v1 - *this) ^ (v2 - *this);
}

Vector3D Vector3D::calculateNormal(Vertex v1, Vertex v2)
{
    return calculateNormal(Vector3D(v1.x, v1.y, v1.z), Vector3D(v2.x, v2.y, v2.z));
}

bool Vector3D::operator ==(Vector3D v)
{
    return  MyMathFunctions::floatComparison(xyz[0],v.xyz[0]) &&
            MyMathFunctions::floatComparison(xyz[1],v.xyz[1]) &&
            MyMathFunctions::floatComparison(xyz[2],v.xyz[2]);
}

const Vector3D &Vector3D::operator =(const Vector3D &v)
{
    xyz[0] = v.xyz[0];
    xyz[1] = v.xyz[1];
    xyz[2] = v.xyz[2];

    return *this;
}

Vector3D Vector3D::operator +(const Vector3D &v) const
{
    return Vector3D(xyz[0] + v.xyz[0],
                    xyz[1] + v.xyz[1],
                    xyz[2] + v.xyz[2]);
}

Vector3D Vector3D::operator -(const Vector3D &v) const
{
    return Vector3D(xyz[0] - v.xyz[0],
                    xyz[1] - v.xyz[1],
                    xyz[2] - v.xyz[2]);
}

float Vector3D::operator *(const Vector3D &v) const
{
    return (xyz[0]*v.xyz[0]) + (xyz[1]*v.xyz[1]) + (xyz[2]*v.xyz[2]);
}

Vector3D Vector3D::operator ^(const Vector3D &v) const
{
    return Vector3D((xyz[1] * v.xyz[2]) - (xyz[2] * v.xyz[1]),
                    (xyz[2] * v.xyz[0]) - (xyz[0] * v.xyz[2]),
                    (xyz[0] * v.xyz[1]) - (xyz[1] * v.xyz[0]));
}

float Vector3D::length() const
{
    return sqrtf(xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2]);
}

void Vector3D::normalize()
{
    if(length() > 0)
        *this = Vector3D(xyz[0], xyz[1], xyz[2]) / length();
}

Vector3D Vector3D::operator *(float c) const
{
    return Vector3D(xyz[0]*c, xyz[1]*c, xyz[2]*c);
}

Vector3D Vector3D::operator /(float c) const
{
    if(c > 0)
        return Vector3D(xyz[0] / c, xyz[1] / c, xyz[2] / c);
    else
        return *this;
}
