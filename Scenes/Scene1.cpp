#include "scene1.h"

#include "xyz.h"
#include "grid.h"
#include "light.h"
#include "octahedronball.h"
#include "bspline.h"
#include <QDebug>

Scene1::Scene1(TriangleSurface *ts)
    :   Scene(ts)
{
    initializeScene();

    //manually set location of ball
    //should maybe be automaticly placed at triangle 0
    //mBall.setLocation(QVector3D(-5.f, 0.f, 0.f));
}

void Scene1::draw(float deltaTime)
{
    Scene::draw(deltaTime);

    if(mPause == true)
        return;

    mBall.getTransform().location.setZ(mTriangleSurface->heightAtLocation(mBall.getLocation().x(), mBall.getLocation().y()) + mBall.getRadius());
    mPhysicsEngine.ballMovement(deltaTime, mBall.getTransform(), mBall.getPhysicsProperties(), mTriangleSurface->getCurrentTriangle().mSurfaceNormal);

}

void Scene1::listObjects()
{    
//    std::vector<float> knots{0,0,0,1,1,1};
//    std::vector<QVector3D> cp{QVector3D(0.433532f, 0.672007f, 1.f),
//                              QVector3D(0.f,0.f,0.f),
//                              QVector3D(1.f,1.f,0.f)};

//    mObjects.push_back(new BSplineCurve(knots, cp, 2));

//    std::vector<float> knots2{0,0,0,1,2,3,3,3};
//    std::vector<QVector3D> cp2{QVector3D(0.f,0.f,0.f),
//                              QVector3D(1.f,0.f,0.f),
//                              QVector3D(1.f,1.f,0.f),
//                              QVector3D(0.f,1.f,0.f),
//                                QVector3D(0.f,0.f,0.f)};

//    mObjects.push_back(new BSplineCurve(knots2, cp2, 2));

    //n = 9
    //d = 2
//    std::vector<float> knots2{0,0,0,0,1,1,2,2,3,3,3,3};
//    std::vector<QVector3D> cp2{QVector3D(0.f,0.f,0.f),
//                              QVector3D(1.f,0.f,0.f),
//                              QVector3D(1.f,1.f,0.f),
//                              QVector3D(0.f,1.f,0.f),
//                              QVector3D(0.f,2.f,0.f),
//                              QVector3D(1.f,2.f,0.f),
//                              QVector3D(1.f,1.f,0.f),
//                              QVector3D(0.f,1.f,0.f),
//                              QVector3D(0.f,0.f,0.f)};

//    mObjects.push_back(new BSplineCurve(knots2, cp2, 3));

   mTriangleSurface = new TriangleSurface("vertex_del1", "index_del1", 1.f);
   mTriangleSurface->run();
   setSurface(mTriangleSurface);
   placeObject(&mBall, {1.f, 4.f});

   //values found printing values with 'C'
   mLockCameraToBall = false;
   mCamera.setLocation(QVector3D(10.f, 4.f, 0.7f));
   mCamera.setYaw(149.2f);
   mCamera.setPitch(-12.2f);
   mCamera.setOffset(14.f);
    //mCamera.setLocation(mTriangleSurface->getVertex(0));
}


