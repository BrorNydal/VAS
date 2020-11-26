#include "scene2.h"
#include "mymathfunctions.h"

#include "indexedtrianglesurface.h"
#include "xyz.h"
#include "grid.h"
#include "beziercurve.h"
#include "octahedronball.h"
#include "light.h"

Scene2::Scene2(IndexedTriangleSurface *ts)
    :   Scene(ts)
{
    initializeScene();

}

void Scene2::draw(float deltaTime)
{
    Scene::draw(deltaTime);

    if(mPause == true)
        return;

    mBall.getTransform().location.setZ(mTriangleSurface->heightAtLocation(mBall.getLocation().x(), mBall.getLocation().y()) + mBall.getRadius());

    if(mTriangleSurface->getTriangle(mBall.getLocation().x(), mBall.getLocation().y()) != nullptr)
        mPhysicsEngine.ballMovement(deltaTime, mBall.getTransform(), mBall.getPhysicsProperties(), mTriangleSurface->getTriangle(mBall.getLocation().x(), mBall.getLocation().y())->mSurfaceNormal);

}

void Scene2::listObjects()
{
    //mTriangleSurface = new IndexedTriangleSurface("test_las", "none", 1.f, true);
    //mTriangleSurface->run();
    //mTriangleSurface->scaleXY(5.f);

    //location found by running program and printing camera location by pressing 'C'
    placeObject(&mBall, {400.824f, 840.358f});
    mCamera.setYaw(200.f);
    mCamera.setPitch(-30.f);
    mCamera.setOffset(50.f);
    //mBall.setLocation({500.f, 500.f,0.f});
}

