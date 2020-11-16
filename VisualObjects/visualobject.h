#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <iostream>
#include "vertex.h"
#include "structs.h"
#include <QMatrix4x4>
#include "shader.h"


class VisualObject : public QOpenGLFunctions_4_1_Core
{
protected:
    std::string mTag;

    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;

    Transform mTransform;
    Buffers mBuffers;

    GLenum mDrawMode = GL_TRIANGLES;
    bool mDrawArrays = false;
    EShader mShaderType{EShader::phong};

public:
    VisualObject()
    {
        initializeOpenGLFunctions();
    }
    ~VisualObject()
    {}

    virtual void init(); //Skal bare ha et parameter, matrix uniform
    virtual void draw(Shader &shader);    

    void setHeight(float h) {mTransform.location.setZ(h);}
    void setLocation(QVector3D loc){mTransform.location = loc;}
    void setRotation(QVector3D rot){mTransform.rotation = rot;}
    void setScale(QVector3D scl)   {mTransform.scale = scl;}

    Transform &getTransform() {return mTransform;}
    QVector3D getLocation() const { return mTransform.location;}
    QVector3D getRotation() const { return mTransform.rotation;}
    QVector3D getScale() const    { return mTransform.scale;}

    EShader getShader() const {return mShaderType;}

    void scale(float factor){mTransform.scale * factor;}
    void move(QVector3D trans){mTransform.location += trans;}

    Vertex getVertex(unsigned int i) {return mVertices[i];}
    unsigned int getVertexCount() const {return mVertices.size();}

private:
    void initializeBuffers_impl(unsigned int *indexData, Vertex *vertexData, unsigned int indexCount, unsigned int vertexCount,
                                Buffers &buffer, GLenum drawing = GL_STATIC_DRAW);
};

#endif // VISUALOBJECT_H
