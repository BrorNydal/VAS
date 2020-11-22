#include "scene2.h"
#include "mymathfunctions.h"

#include "indexedtrianglesurface.h"
#include "xyz.h"
#include "grid.h"
#include "beziercurve.h"
#include "octahedronball.h"
#include "light.h"

Scene2::Scene2()
{
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
    mTriangleSurface = new IndexedTriangleSurface("test_las", "none", 1.f, true);
    mTriangleSurface->run();

    mBall.setLocation({10.f,10.f,0.f});
}

