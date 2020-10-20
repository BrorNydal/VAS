#ifndef OPPGAVE_H
#define OPPGAVE_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>

#include "VisualObjects/visualobject.h"
#include "shader.h"

class Scene : public QOpenGLFunctions_4_1_Core
{
protected:
    //Objects needed for current task
    std::vector<VisualObject*> mObjects;    

    //FOR ALL VECTORS
    //Index 0 = Plain Shader
    //Index 1 = Phong Shader

    //SHADERS
    std::vector<Shader*> mShaders;
    Shader *mPlainShader;
    Shader *mPhongShader;

    //UNIFORMS

    //Transformation Matrix Uniforms
    std::vector<GLint> mTransformationUniforms;
    GLint mPlainTransformMatrixUniform;
    GLint mPhongTransformMatrixUniform;

    //Camera Uniforms
    std::vector<GLint> mViewUniforms;
    std::vector<GLint> mProjectionUniforms;

    GLint mPlainViewUniform;
    GLint mPlainProjectionUniform;

    GLint mPhongViewUniform;
    GLint mPhongProjectionUniform;
    GLint mViewPosition;

    //Light Uniforms
    GLint mLightPositionUniform;
    GLint mLightIntensityUniform;
    GLint mLightColorUniform;

public:
    Scene();
    ~Scene();

    void initializeScene();
    //Draws all elements of mObjects
    virtual void draw();

    //Creates and pushes objects to mObjects-vector
    virtual void listObjects() = 0;
    virtual void setUniforms() = 0;
    //Calls createObjects() and init() in order
    void createAndInitialize();

    //Camera
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;

    //Has scene been initialized
    bool mInitialized = false;

    //If we want to spin our scene
    bool mTurnTable = false;

protected:
    //Initializes all elements of mObjects
    void init();

    virtual void setTransformations() = 0;
};

#endif // OPPGAVE_H
