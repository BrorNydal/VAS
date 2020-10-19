#ifndef OPPGAVE_H
#define OPPGAVE_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include <iostream>
#include "StaticFunctions.h"
#include "VisualObjects/visualobject.h"
#include "shader.h"

class Scene : public QOpenGLFunctions_4_1_Core
{
protected:
    //Objects for this scene
    std::vector<VisualObject*> mObjects;
    std::vector<VisualObject*> mEditorObjects;
    std::vector<VisualObject*> mPlayObjects;

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

    //STANDARD SCENE-OBJECTS

    //Editor mode
    VisualObject *mXYZ;
    VisualObject *mGrid;

    //Playmode
    class Light * mLight;


public:
    Scene();
    ~Scene();

    void initializeScene();
    //Draws all elements of mObjects
    virtual void draw();

    //Creates and pushes objects to mObjects-vector
    virtual void listObjects();
    virtual void setUniforms();
    //Calls createObjects() and init() in order
    void createAndInitialize();

    //Camera
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;

protected:
    //Initializes all elements of mObjects
    void init();

    virtual void setTransformations();
    void editorModeSwitch() {BoolianFunctions::boolianSwitch(mEditorMode);}

private:
    bool mEditorMode;
};

#endif // OPPGAVE_H
