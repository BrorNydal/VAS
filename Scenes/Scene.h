#ifndef OPPGAVE_H
#define OPPGAVE_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "structs.h"
#include "visualobject.h"
#include "light.h"
#include "xyz.h"
#include "grid.h"
#include "rollingball.h"
#include "indexedtrianglesurface.h"
#include "shader.h"
#include "camera.h"
#include "physicsengine.h"


/*
 * P - Play / Pause
 * WASD - camera movement.
 * IJKL - player movement.
 * Right Mouse Button enables camera movement.
 * E - locks or unlocks camera from player character
 * Q - resets current scene
 * Space - next scene
 */
class Scene : public QOpenGLFunctions_4_1_Core
{
protected:
    //Objects needed for current task
    std::vector<VisualObject*> mObjects;    

    //shaders needed, plain, phong
    std::map<EShader, Shader*> mShaders;

    //Has scene been initialized
    bool mInitialized = false;
    bool mPause = false;

    PhysicsEngine mPhysicsEngine;

    TriangleSurface *mTriangleSurface = nullptr;
    Camera mCamera;
    bool mLockCameraToBall = true; //When true, camera follows ball, when false move camera with WASD + shift/ctrl
    Light mLight; //Directional
    XYZ mXYZ;
    Grid mGrid;
    RollingBall mBall; //Ball to simulate physics and move

    std::map<VisualObject*, QVector2D> mDefaultPositions;

public:
    Scene(TriangleSurface *ts = nullptr);
    ~Scene();

    void initializeScene();
    virtual void reset();

    //Draws all elements of mObjects
    virtual void draw(float deltaTime);

    //Creates and pushes objects to mObjects-vector
    virtual void listObjects() = 0;

    void placeObject(VisualObject *obj, QVector2D loc);

    void togglePause();
    bool isPaused() const;
    void setSurface(TriangleSurface *surface);

    bool hasInitialized() const {return mInitialized;}
    Shader *getShader(EShader type);
    Camera &getCamera();
    void toggleCameraLookAtBall();
    RollingBall &getBall();


protected:
    //Initializes all elements of mObjects
    void initializeObjects();
};

#endif // OPPGAVE_H
