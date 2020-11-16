#ifndef OPPGAVE_H
#define OPPGAVE_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "structs.h"
#include "visualobject.h"
#include "light.h"
#include "xyz.h"
#include "grid.h"
#include "octahedronball.h"
#include "indexedtrianglesurface.h"
#include "shader.h"
#include "camera.h"
#include "physicsengine.h"



class Scene : public QOpenGLFunctions_4_1_Core
{
protected:
    //Objects needed for current task
    std::vector<VisualObject*> mObjects;

    //shaders needed, plain, phong
    std::map<EShader, Shader*> mShaders;

    //Has scene been initialized
    bool mInitialized = false;

    PhysicsEngine mPhysicsEngine;

    IndexedTriangleSurface *mTriangleSurface = nullptr;
    Camera mCamera;
    Light mLight;
    XYZ mXYZ;
    Grid mGrid;
    OctahedronBall mBall;

public:
    Scene();
    ~Scene();

    void initializeScene();
    //Draws all elements of mObjects
    virtual void draw(float deltaTime);

    //Creates and pushes objects to mObjects-vector
    virtual void listObjects() = 0;

    bool hasInitialized() const {return mInitialized;}
    Shader *getShader(EShader type);
    Camera &getCamera();


protected:
    //Initializes all elements of mObjects
    void initializeObjects();
};

#endif // OPPGAVE_H
