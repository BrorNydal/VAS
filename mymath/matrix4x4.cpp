#include "matrix4x4.h"
#include "vector4d.h"
#include <cmath>
#include <qmath.h>



Matrix4x4::Matrix4x4()
{
    srand(static_cast<unsigned int>(clock()));
    initializeMatrix();
}

void Matrix4x4::randomizeMatrix()
{
    for(auto i = 0; i < matrixSize; i++)
        for(auto j = 0; j < matrixSize; j++)
            xy[i][j] = rand() % 10;
}

void Matrix4x4::initializeMatrix()
{
    for(auto i = 0; i < matrixSize; i++)
        for(auto j = 0; j < matrixSize; j++)
            xy[i][j] = 0;
}

void Matrix4x4::setToIdentity()
{
    for(auto i = 0; i < matrixSize; i++)
    {
        for(auto j = 0; j < matrixSize; j++)
        {
            if(i == j)
                xy[i][j] = 1;
            else
                xy[i][j] = 0;
        }
    }
}

Matrix4x4 Matrix4x4::operator =(Matrix4x4 m)
{
    for(auto i = 0; i < matrixSize; i++)
        for(auto j = 0; j < matrixSize; j++)
            xy[i][j] = m.xy[i][j];

    return *this;
}

Matrix4x4 Matrix4x4::operator *(float c)
{
    for(auto i = 0; i < matrixSize; i++)
        for(auto j = 0; j < matrixSize; j++)
            xy[i][j] = xy[i][j] * c;

    return *this;
}

Vector4D Matrix4x4::operator *(Vector4D v)
{
    float xyz[matrixSize];
    for(auto i = 0; i < matrixSize; i++)
    {        
        for(auto j = 0; j < matrixSize; j++)
        {
            if(j < 3)
                xyz[j] = xy[i][j] * v.getVector3D().getXYZ(j);
            else
                xyz[j] = xy[i][j] * v.getdimension4Value();
        }
    }

    return Vector4D(Vector3D(xyz[0], xyz[1], xyz[2]), xyz[3]);
}

Matrix4x4 Matrix4x4::operator *(Matrix4x4 m)
{
    Matrix4x4 matrix;

    for(auto i = 0; i < matrixSize; i++)
        for(auto j = 0; j < matrixSize; j++)
            for(auto k = 0; k < matrixSize; k++)
                matrix.xy[i][j] += xy[i][k] * m.xy[k][j];

    return matrix;
}

void Matrix4x4::scale(float sX, float sY, float sZ)
{
    Matrix4x4 m;

    m.xy[0][0] = sX;    //Set
    m.xy[1][1] = sY;
    m.xy[2][2] = sZ;
    m.xy[3][3] = 1;

    *this = *this * m;
}

void Matrix4x4::rotate(Vector3D v, float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    Matrix4x4 m;
    m.setToIdentity();

    //X-AKSE
    if(v.getXYZ(0) > 0.f)
    {
        m.xy[1][1] = c;
        m.xy[2][2] = c;
        m.xy[1][2] = (-s);
        m.xy[2][1] = s;
    }

    //Y-AKSE    -   Aksen som peker opp
    else if(v.getXYZ(1) > 0.f)
    {
        m.xy[0][0] = c;
        m.xy[2][0] = (-s);
        m.xy[0][2] = s;
        m.xy[2][2] = c;
    }

    //Z-AKSE    -   Er ikke aksen som peker opp
    else
    {
        m.xy[0][0] = c;
        m.xy[0][1] = s;
        m.xy[1][0] = (-s);
        m.xy[1][1] = c;
    }

    *this = *this * m;
}

void Matrix4x4::translate(float tX, float tY, float tZ)
{
    Matrix4x4 m;
    m.setToIdentity();

    m.xy[0][3] = tX;
    m.xy[1][3] = tY;
    m.xy[2][3] = tZ;

    *this = *this * m;
}
