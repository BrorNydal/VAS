#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <iostream>
#include "vertex.h"
#include "MyMath/vector3d.h"
#include "MyMath/vector2d.h"
#include <QMatrix4x4>

class VisualObject : public QOpenGLFunctions_4_1_Core
{
public:
    VisualObject()
        :   mShaderIndex(0)
    {}
    ~VisualObject() {
        glDeleteVertexArrays( 1, &mVAO );
           glDeleteBuffers( 1, &mVBO );
    }

    virtual void init() = 0; //Skal bare ha et parameter, matrix uniform
    virtual void draw() = 0;

    bool modelView;

    void setHeight(float h) {mMatrix(2,3) = h;}
    void setPosition(Vector3D pos){
        mMatrix(0, 3) = pos.x();
        mMatrix(1, 3) = pos.y();
        mMatrix(2, 3) = pos.z();
    }
    Vector3D getPosition() {
        return Vector3D(mMatrix(0, 3),
                        mMatrix(1, 3),
                        mMatrix(2, 3));
    }
    void scale(float factor){
        mMatrix.scale(factor);
    }
    void rotateZ(float factor){
        mMatrix.rotate(factor, QVector3D(0.f, 0.f, 1.f));
    }

    void setTransformMatrixUniform(GLint matrixUniform) {mMatrixUniform = matrixUniform;}
    QMatrix4x4 getTransformationMatrix() {return mMatrix;}

    unsigned int getShaderIndex() {return mShaderIndex;}
    Vertex getVertex(unsigned int i) {return mVertices[i];}
protected:
    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0};
    GLint mMatrixUniform{0};
    QMatrix4x4 mMatrix;

    GLenum mDrawMode;
    unsigned int mShaderIndex;
};

#endif // VISUALOBJECT_H
