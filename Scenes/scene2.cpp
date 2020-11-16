#include "scene2.h"
#include "mymathfunctions.h"

#include "indexedtrianglesurface.h"
#include "xyz.h"
#include "grid.h"
#include "beziercurve.h"
#include "octahedronball.h"
#include "light.h"
#include "contourline.h"

Scene2::Scene2()
{
}

void Scene2::draw(float deltaTime)
{
    Scene::draw(deltaTime);

}

void Scene2::listObjects()
{

    //mCamera.setLocation(mTriangleSurface->getVertex(0));
    qDebug() << "cam loc :" << mCamera.getLocation();
    qDebug() << (int)mTriangleSurface->getVertex(0).x << (int)mTriangleSurface->getVertex(0).y << (int)mTriangleSurface->getVertex(0).z;
}

