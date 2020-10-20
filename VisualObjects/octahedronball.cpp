#define _USE_MATH_DEFINES

#include "octahedronball.h"

#include <QVector3D>

#include <iostream>
#include <sstream>
#include <cmath>

OctahedronBall::OctahedronBall(float radius, int rekursions)
    :   mRadius(radius), mRekursions(rekursions), mIndex(0)
{
    mVertices.reserve(3 * 8 * pow(4, mRekursions));
    oktaederUnitBall();

    //Use Phong Shader
    mShaderIndex = 1;
}

void OctahedronBall::init()
{
    mMatrix.setToIdentity();

   initializeOpenGLFunctions();

   glGenVertexArrays(1, &mVAO);

   //what object to draw
   glBindVertexArray(mVAO);

   glGenBuffers(1, &mVBO);
   glBindBuffer(GL_ARRAY_BUFFER, mVBO);

   //Vertex Buffer Object to hold vertices - VBO
   glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

   glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)(3 * sizeof(GLfloat)));
   glEnableVertexAttribArray(1);
}

void OctahedronBall::draw()
{
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.data());
    glBindVertexArray( mVAO );
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(mVertices.size()));
}

void OctahedronBall::createTriangel(Vector3D &v1, Vector3D &v2, Vector3D &v3)
{
    Vertex v;

    Vector3D normal = v1 - getPosition();

    v.setXYZ(v1.x(), v1.y(), v1.z());    // koordinater v.x = v1.x, v.y=v1.y, v.z=v1.z
    v.setRGB(normal.x(), normal.y(), normal.z());	// rgb
    mVertices.push_back(v);

    normal = v2 - getPosition();
    v.setXYZ(v2.x(), v2.y(), v2.z());
    v.setRGB(normal.x(), normal.y(), normal.z());
    mVertices.push_back(v);

    normal = v3 - getPosition();
    v.setXYZ(v3.x(), v3.y(), v3.z());
    v.setRGB(normal.x(), normal.y(), normal.z());
    mVertices.push_back(v);

}

void OctahedronBall::subDivide(Vector3D &a, Vector3D& b, Vector3D& c, int n)
{
    if (n>0) {
        Vector3D v1 = a+b; v1.normalize();
        Vector3D v2 = a+c; v2.normalize();
        Vector3D v3 = c+b; v3.normalize();
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
    Vector3D v0(0, 0, mRadius);
    Vector3D v1(mRadius, 0, 0);
    Vector3D v2(0, mRadius, 0);
    Vector3D v3(-mRadius, 0, 0);
    Vector3D v4(0, -mRadius, 0);
    Vector3D v5(0, 0, -mRadius);

    subDivide(v0, v1, v2, mRekursions);
    subDivide(v0, v2, v3, mRekursions);
    subDivide(v0, v3, v4, mRekursions);
    subDivide(v0, v4, v1, mRekursions);
    subDivide(v5, v2, v1, mRekursions);
    subDivide(v5, v3, v2, mRekursions);
    subDivide(v5, v4, v3, mRekursions);
    subDivide(v5, v1, v4, mRekursions);
}
