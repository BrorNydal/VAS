#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "vector3d.h"

class Vector2D
{
private:
    static const unsigned int vectorSize = 2;
    float xy[vectorSize];
public:
    Vector2D();
    Vector2D(float x, float y);

    float x() const {return xy[0];}
    float y() const {return xy[1];}

    const Vector2D& operator = (const Vector2D& v);
    Vector2D operator - (const Vector2D& v) const;
    Vector2D operator / (float v) const;
    Vector3D operator ^ (const Vector2D& v) const;  // Cross product
    float operator * (const Vector2D& v) const;
    float length() const;
    void normalize();

    Vector3D barycentricCoordinates(const Vector2D &v1, const Vector2D &v2, const Vector2D &v3);
};

#endif // VECTOR2D_H
