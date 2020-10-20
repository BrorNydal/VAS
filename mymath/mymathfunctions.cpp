#include "mymathfunctions.h"
#include "triangle.h"

#include <cmath>

Vector3D MyMathFunctions::centroidOfTriangle(OldTriangle *triangle)
{
    Vector3D centroid;

    for(unsigned int i = 0; i < triangle->mPositions.size(); i++)
        centroid = centroid + triangle->mPositions[i];

    return centroid = centroid / triangle->mPositions.size();
}

Vector3D MyMathFunctions::centroidOfTriangle(Vector3D v1, Vector3D v2, Vector3D v3)
{
    return (v1+v2+v3)/3;
}

Vector3D MyMathFunctions::average3DVector(std::vector<Vector3D> vs)
{
    Vector3D averageVector;

    for(unsigned int i = 0; i < vs.size(); i++)
        averageVector = averageVector + vs[i];

    return averageVector/ vs.size();
}

bool MyMathFunctions::floatComparison(float a, float b, float accuracy)
{
    return std::fabs(a - b) < accuracy;
}



