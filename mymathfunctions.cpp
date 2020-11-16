#include "mymathfunctions.h"
#include "triangle.h"

#include <cmath>

QVector3D MyMathFunctions::centroidOfTriangle(OldTriangle *triangle)
{
    QVector3D centroid;

    for(unsigned int i = 0; i < triangle->mPositions.size(); i++)
        centroid = centroid + triangle->mPositions[i];

    return centroid = centroid / triangle->mPositions.size();
}

QVector3D MyMathFunctions::centroidOfTriangle(QVector3D v1, QVector3D v2, QVector3D v3)
{
    return (v1+v2+v3)/3;
}

QVector3D MyMathFunctions::average3DVector(std::vector<QVector3D> vs)
{
    QVector3D averageVector;

    for(unsigned int i = 0; i < vs.size(); i++)
        averageVector = averageVector + vs[i];

    return averageVector/ vs.size();
}

bool MyMathFunctions::floatComparison(float a, float b, float accuracy)
{
    return std::fabs(a - b) < accuracy;
}



