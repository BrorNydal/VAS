#ifndef OCTAHEDRONBALL_H
#define OCTAHEDRONBALL_H

#include "VisualObjects/visualobject.h"


class OctahedronBall : public VisualObject
{
    float mRadius;
    int mRekursions;
    int mIndex;               // brukes i rekursjon, til å bygge m_vertices

public:
    OctahedronBall(float radius = 1.f, int rekursions = 0);

    virtual void init() override;
    virtual void draw() override;

private:
    void createTriangel(Vector3D &v1, Vector3D &v2, Vector3D &v3);
    void subDivide(Vector3D &a, Vector3D& b, Vector3D& c, int n);
    void oktaederUnitBall();

};

#endif // OCTAHEDRONBALL_H
