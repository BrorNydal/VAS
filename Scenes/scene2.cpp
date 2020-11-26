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

    for(auto &physicObject : mPhysicsObjects)
    {
        physicObject->getTransform().location.setZ(mTriangleSurface->heightAtLocation(physicObject->getLocation().x(), physicObject->getLocation().y()) + physicObject->getRadius());

        if(mTriangleSurface->getTriangle(physicObject->getLocation().x(), physicObject->getLocation().y()) != nullptr)
            mPhysicsEngine.ballMovement(deltaTime, physicObject->getTransform(), physicObject->getPhysicsProperties(), mTriangleSurface->getTriangle(physicObject->getLocation().x(), physicObject->getLocation().y())->mSurfaceNormal);
    }

    CollisionResult collision;

    for(unsigned int i = 0; i < mPhysicsObjects.size(); i++)
    {
        RollingBall *po1 = mPhysicsObjects[i];

        for(unsigned int k = i + 1; k < mPhysicsObjects.size(); k++)
        {
            RollingBall *po2 = mPhysicsObjects[k];

            if(po2 != po1)
            {
                collision = mPhysicsEngine.CheckCollision(Sphere(), po1->getTransform(), Sphere(), po2->getTransform());

                if(collision.collision)
                {
                    mPhysicsEngine.handleCollision(Sphere(), po1->getPhysicsProperties(), po1->getTransform(), Sphere(), po2->getPhysicsProperties(), po2->getTransform());
                }
            }
        }
    }
}

void Scene2::listObjects()
{
    //mTriangleSurface = new IndexedTriangleSurface("test_las", "none", 1.f, true);
    //mTriangleSurface->run();
    //mTriangleSurface->scaleXY(5.f);

    RollingBall *p0 = new RollingBall();
    placeObject(p0, {400.f, 930.f});
    mObjects.push_back(p0);
    mPhysicsObjects.push_back(p0);

    RollingBall *p1 = new RollingBall();
    placeObject(p1, {420.f, 910.f});
    mObjects.push_back(p1);
    mPhysicsObjects.push_back(p1);

    RollingBall *p2 = new RollingBall();
    placeObject(p2, {410.f, 860.f});
    mObjects.push_back(p2);
    mPhysicsObjects.push_back(p2);

    RollingBall *p3 = new RollingBall();
    placeObject(p3, {390.f, 860.f});
    mObjects.push_back(p3);
    mPhysicsObjects.push_back(p3);

    RollingBall *p4 = new RollingBall();
    placeObject(p4, {450.f, 830.f});
    mObjects.push_back(p4);
    mPhysicsObjects.push_back(p4);

    RollingBall *p5 = new RollingBall();
    placeObject(p5, {843.527, 995.542});
    mObjects.push_back(p5);
    mPhysicsObjects.push_back(p5);

    RollingBall *p6 = new RollingBall();
    placeObject(p6, {797.199, 875.6});
    mObjects.push_back(p6);
    mPhysicsObjects.push_back(p6);

    RollingBall *p7 = new RollingBall();
    placeObject(p7, {712.546, 1067.93});
    mObjects.push_back(p7);
    mPhysicsObjects.push_back(p7);

    RollingBall *p8 = new RollingBall();
    placeObject(p8, {832.278, 1100.24});
    mObjects.push_back(p8);
    mPhysicsObjects.push_back(p8);

    mPhysicsObjects.push_back(&mBall);

    //location found by running program and printing camera location by pressing 'C'
    placeObject(&mBall, {400.824f, 840.358f});
    mCamera.setYaw(190.8f);
    mCamera.setPitch(-28.f);
    mCamera.setOffset(93.f);
    //mBall.setLocation({500.f, 500.f,0.f});
}

