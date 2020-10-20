#include "vector2d.h"

#include <cmath>

Vector2D::Vector2D()
{

}

Vector2D::Vector2D(float x, float y)
{
    xy[0] = x;
    xy[1] = y;
}

const Vector2D &Vector2D::operator =(const Vector2D &v)
{
    xy[0] = v.xy[0];
    xy[1] = v.xy[1];

    return *this;
}

Vector2D Vector2D::operator -(const Vector2D &v) const
{
    return Vector2D(xy[0] - v.xy[0], xy[1] - v.xy[1]);
}

Vector2D Vector2D::operator /(float v) const
{
    return Vector2D(xy[0] / v, xy[1] / v);
}

Vector3D Vector2D::operator ^(const Vector2D &v) const
{
    Vector3D v1 = Vector3D(xy[0], xy[1], 0.f);
    Vector3D v2 = Vector3D(v.xy[0], v.xy[1], 0.f);

    return Vector3D((v1.y() * v2.z()) - (v1.z() * v2.y()),
                    (v1.z() * v2.x()) - (v1.x() * v2.z()),
                    (v1.x() * v2.y()) - (v1.y() * v2.x()));
}

float Vector2D::operator *(const Vector2D &v) const
{
    return (xy[0] * v.xy[0]) + (xy[1] * v.xy[1]);
}

float Vector2D::length() const
{
    return sqrtf((xy[0] * xy[0]) + (xy[1] * xy[1]));
}

void Vector2D::normalize()
{
    if(length() != 0.f)
        *this = *this/length();
}

Vector3D Vector2D::barycentricCoordinates(const Vector2D &v1, const Vector2D &v2, const Vector2D &v3)
{
    Vector2D v12 = v2-v1;   //Vec2
    Vector2D v13 = v3-v1;   //Vec2

    Vector3D n = v12^v13;
    float areal123 = n.length();

    Vector3D baryc; //return value

    //u
    Vector2D p = v2 - *this;    //Vec2
    Vector2D q = v3 - *this;    //Vec2
    n = p^q;
    baryc.setX(n.z() / areal123);

    //v
    p = v3 - *this;
    q = v1 - *this;
    n = p^q;
    baryc.setY(n.z() / areal123);

    //w
    p = v1 - *this;
    q = v2 - *this;
    n = p^q;
    baryc.setZ(n.z() / areal123);

    return baryc;
}
