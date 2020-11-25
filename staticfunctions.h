#ifndef STATICFUNCTIONS_H
#define STATICFUNCTIONS_H

#include <QVector3D>
#include <QVector2D>
#include <cmath>

static bool CompareFloats(float a, float b)
{
    return fabs(a - b) < __FLT_EPSILON__;
}

static QVector3D BarycentricCoordinates(const QVector2D &v, const QVector2D &v1, const QVector2D &v2, const QVector2D &v3){
    QVector2D v12 = v2-v1;   //Vec2
    QVector2D v13 = v3-v1;   //Vec2

    QVector3D n =  QVector3D::crossProduct(v12, v13);
    float areal123 = n.length();

    QVector3D baryc; //return value

    //u
    QVector2D p = v2 - v;    //Vec2
    QVector2D q = v3 - v;    //Vec2
    n = QVector3D::crossProduct(p, q);
    baryc.setX(n.z() / areal123);

    //v
    p = v3 - v;
    q = v1 - v;
    n = QVector3D::crossProduct(p, q);
    baryc.setY(n.z() / areal123);

    //w
    p = v1 - v;
    q = v2 - v;
    n = QVector3D::crossProduct(p, q);
    baryc.setZ(n.z() / areal123);

    return baryc;
}

static QVector3D CalculateTriangleNormal(const QVector3D &v1, const QVector3D &v2, const QVector3D &v3)
{
    return QVector3D::crossProduct(v2 - v1, v3 - v1);
}

#endif // STATICFUNCTIONS_H
