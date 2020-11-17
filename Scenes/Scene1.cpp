#include "scene1.h"

#include "xyz.h"
#include "grid.h"
#include "trianglesurface.h"
#include "light.h"
#include "beziercurve.h"
#include "octahedronball.h"
#include "contourline.h"
#include "bspline.h"
#include <QDebug>

Scene1::Scene1()
{
    initializeScene();

    //manually set location of ball
    //should maybe be automaticly placed at triangle 0
    //mBall.setLocation(QVector3D(-5.f, 0.f, 0.f));
    mCamera.setLocation({0.5f,0.5f,0.1f});
}

void Scene1::draw(float deltaTime)
{
    Scene::draw(deltaTime);

    //mBall.getTransform().location.setZ(mTriangleSurface->barycentricHeightSearch(QVector2D(mBall.getLocation().x(), mBall.getLocation().y())) + mBall.getRadius());
    //mPhysicsEngine.ballMovement(deltaTime, mBall.getTransform(), mBall.getPhysicsProperties(), mTriangleSurface->getCurrentTriangle().mSurfaceNormal);
    mBall.getTransform().location.setZ(mTriangleSurface->heightAtLocation(mBall.getLocation().x(), mBall.getLocation().y()) + mBall.getRadius());
}

void Scene1::listObjects()
{    
//    std::vector<float> knots{0,0,0,1,2,2,2};
//    std::vector<QVector3D> cp{QVector3D(0.f,0.f,0.f),
//                              QVector3D(1.f,0.f,0.f),
//                              QVector3D(1.f,1.f,0.f),
//                              QVector3D(0.f,1.f,0.f)};

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

   mTriangleSurface = new IndexedTriangleSurface("test_las", "none", 1.f, true);
   //mTriangleSurface = new IndexedTriangleSurface("vertex_del1", "index_del1", 1.f/1.f);
   mTriangleSurface->run();
   mBall.setLocation(QVector3D(200.f, 200.f, 0.f));
    //mCamera.setLocation(mTriangleSurface->getVertex(0));
}


