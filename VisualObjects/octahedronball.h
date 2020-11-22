#ifndef OCTAHEDRONBALL_H
#define OCTAHEDRONBALL_H

#include "visualobject.h"

#include "structs.h"

class OctahedronBall : public VisualObject
{
    float mRadius;
    int mRekursions;
    int mIndex;               // brukes i rekursjon, til å bygge m_vertices

public:
    OctahedronBall(float radius = 1.f, int rekursions = 3);

    virtual void draw(Shader &shader) override;
    float getRadius() const;


private:
    void createTriangel(QVector3D &v1, QVector3D &v2, QVector3D &v3);
    void subDivide(QVector3D &a, QVector3D& b, QVector3D& c, int n);
    void oktaederUnitBall();


};

#endif // OCTAHEDRONBALL_H
