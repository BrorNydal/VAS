#include "Scenes/Scene1.h"

#include "VisualObjects/terrain.h"
#include "VisualObjects/octahedronball.h"
#include "VisualObjects/light.h"
#include "VisualObjects/objfile.h"

Scene1::Scene1()
    :   mTerrain(new Terrain(10, 10)), mPlayerObject(new ObjFile(std::string("TestObject")))
{
    initializeOpenGLFunctions();

    //Camera Position
    mViewMatrix(0, 3) = 0.f;
    mViewMatrix(1, 3) = 0.f;
    mViewMatrix(2, 3) = -8.f;
}

void Scene1::draw()
{
    Scene::draw();
}

void Scene1::setUniforms()
{
    Scene::setUniforms();

    mTerrain->setTransformMatrixUniform(mPhongTransformMatrixUniform);
    mPlayerObject->setTransformMatrixUniform(mPhongTransformMatrixUniform);
}

void Scene1::listObjects()
{
    Scene::listObjects();

    mObjects.push_back(mTerrain);
    mObjects.push_back(mPlayerObject);
}

void Scene1::setTransformations()
{
    Scene::setTransformations();

    mLight->setPosition(Vector3D(3, 3, 5.f));
}


