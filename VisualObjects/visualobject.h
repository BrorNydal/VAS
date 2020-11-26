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

    //drawing buffers
    Buffers mBuffers;

    GLenum mDrawMode = GL_TRIANGLES;
    bool mDrawArrays = false;
    EShader mShaderType{EShader::phong};

    //While true: rendered and is active through update each frame.
    //While false: is not rendered and will not be updated each frame.
    bool mActive{true};

public:
    VisualObject()
    {
        initializeOpenGLFunctions();
    }
    ~VisualObject()
    {}

    void Activate() {mActive = true;}
    void Deactivate() {mActive = false;}
    bool IsActive() const {return mActive;}

    virtual void init(); //Skal bare ha et parameter, matrix uniform

    /**
     * Renders this object using given shader.
     * @param shader.
     */
    virtual void draw(Shader &shader);


    void setHeight(float h) {mTransform.location.setZ(h);}
    void setLocation(QVector3D loc){mTransform.location = loc;}
    void setRotation(QVector3D rot){mTransform.rotation = rot;}
    void setScale(QVector3D scl)   {mTransform.scale = scl;}
    void setScale(float s) {mTransform.scale = QVector3D(s, s, s);}

    Transform &getTransform() {return mTransform;}
    QVector3D getLocation() const { return mTransform.location;}
    QVector3D getRotation() const { return mTransform.rotation;}
    QVector3D getScale() const    { return mTransform.scale;}

    EShader getShader() const {return mShaderType;}

    void scale(float factor){mTransform.scale *= factor;}
    void scaleXY(float factor){mTransform.scale[0]*=factor; mTransform.scale[1]*=factor;}
    void move(QVector3D trans){mTransform.location += trans;}

    Vertex getVertex(unsigned int i) {return mVertices[i];}
    unsigned int getVertexCount() const {return mVertices.size();}

protected:
    /**
     * Initializes buffers.
     * @param indexData pointer to first element in indexvector.
     * @param vertexData pointer to first element in vertexvector.
     * @param indexCount indexvector size.
     * @param vertexCount vertexvector size.
     * @param buffer a struct including vao etc..
     * @param drawing if the vertices can be changed dynamically.
     */
    void initializeBuffers_impl(unsigned int *indexData, Vertex *vertexData, unsigned int indexCount, unsigned int vertexCount,
                                Buffers &buffer, GLenum drawing = GL_STATIC_DRAW);
};

#endif // VISUALOBJECT_H
