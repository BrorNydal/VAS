#define _USE_MATH_DEFINES

#include "octahedronball.h"

#include <QVector3D>

#include <iostream>
#include <sstream>
#include <cmath>

OctahedronBall::OctahedronBall(float radius, int rekursions)
    :   mRadius(radius), mRekursions(rekursions), mIndex(0)
{
    //mVertices.reserve(3 * 8 * pow(4, mRekursions));
    oktaederUnitBall();
    mDrawArrays = true;
}

void OctahedronBall::draw(Shader &shader)
{
    shader.uniform3f("color", 1.f, 0.f, 0.f);
    VisualObject::draw(shader);
}

float OctahedronBall::getRadius() const
{
    return mRadius;
}

void OctahedronBall::createTriangel(QVector3D &v1, QVector3D &v2, QVector3D &v3)
{
    Vertex v;

    QVector3D normal = v1 - getLocation();

    v.setXYZ(v1.x(), v1.y(), v1.z());    // koordinater v.x = v1.x, v.y=v1.y, v.z=v1.z
    v.setRGB(normal.x(), normal.y(), normal.z());	// rgb
    mVertices.push_back(v);

    normal = v2 - getLocation();
    v.setXYZ(v2.x(), v2.y(), v2.z());
    v.setRGB(normal.x(), normal.y(), normal.z());
    mVertices.push_back(v);

    normal = v3 - getLocation();
    v.setXYZ(v3.x(), v3.y(), v3.z());
    v.setRGB(normal.x(), normal.y(), normal.z());
    mVertices.push_back(v);
}

void OctahedronBall::subDivide(QVector3D &a, QVector3D& b, QVector3D& c, int n)
{
    if (n>0) {
        QVector3D v1 = a+b; v1.normalize();
        QVector3D v2 = a+c; v2.normalize();
        QVector3D v3 = c+b; v3.normalize();
        subDivide(a, v1, v2, n-1);
        subDivide(c, v2, v3, n-1);
        subDivide(b, v3, v1, n-1);
        subDivide(v3, v2, v1, n-1);
    }
    else {
        createTriangel(a, b, c);
    }
}

void OctahedronBall::oktaederUnitBall()
{
    QVector3D v0(0, 0, mRadius);
    QVector3D v1(mRadius, 0, 0);
    QVector3D v2(0, mRadius, 0);
    QVector3D v3(-mRadius, 0, 0);
    QVector3D v4(0, -mRadius, 0);
    QVector3D v5(0, 0, -mRadius);

    subDivide(v0, v1, v2, mRekursions);
    subDivide(v0, v2, v3, mRekursions);
    subDivide(v0, v3, v4, mRekursions);
    subDivide(v0, v4, v1, mRekursions);
    subDivide(v5, v2, v1, mRekursions);
    subDivide(v5, v3, v2, mRekursions);
    subDivide(v5, v4, v3, mRekursions);
    subDivide(v5, v1, v4, mRekursions);
}


