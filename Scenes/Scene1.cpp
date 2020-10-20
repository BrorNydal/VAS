#include "Scenes/Scene1.h"

#include "VisualObjects/xyz.h"
#include "VisualObjects/grid.h"
#include "VisualObjects/trianglesurface.h"
#include "VisualObjects/light.h"
#include "VisualObjects/beziercurve.h"
#include "VisualObjects/octahedronball.h"
#include "VisualObjects/contourline.h"

#include <QDebug>

Scene1::Scene1()
    :   mXYZ(new XYZ()), mGrid(new Grid()), mTriangleSurface(new TriangleSurface("datasett_5_2_4")),
        mLight(new Light()), mBCLightMover(new BezierCurve()), mSimulationObject(new OctahedronBall(1, 3)),
        mContourLine(new ContourLine(mTriangleSurface, 1.5, false))
{
    initializeOpenGLFunctions();

    mContourLine->createContourLine();

    //Moving light
    std::vector<Vector3D> lightCurve;
    lightCurve.push_back(Vector3D(0,    0,  4));
    lightCurve.push_back(Vector3D(-3.5,    2.5,  4));
    lightCurve.push_back(Vector3D(0,    5, 4));
    lightCurve.push_back(Vector3D(5,   5, 4));
    lightCurve.push_back(Vector3D(5,   0,  4));
    lightCurve.push_back(Vector3D(0,    0,  4));

    mBCLightMover->setControlPoints(lightCurve);

    //Set object moving on surface
    mTriangleSurface->setSimulationObject(mSimulationObject);

    //Camera Position
    mViewMatrix(0, 3) = 0.f;
    mViewMatrix(1, 3) = 0.f;
    mViewMatrix(2, 3) = -8.f;
}

void Scene1::draw()
{
    Scene::draw();

    mLight->setPosition(Vector3D(mBCLightMover->getLocationOnBezierCurve(mLightMoverIndex).x(),
                                 mBCLightMover->getLocationOnBezierCurve(mLightMoverIndex).y(),
                                 mBCLightMover->getLocationOnBezierCurve(mLightMoverIndex).z()));

    mLightMoverIndex += mLightSpeed;

    if(mLightMoverIndex >= mBCLightMover->getBezierCurveVectorSize() - mLightSpeed || mLightMoverIndex <= 0)
    {
        mLightMoverIndex = 0;
    }
}

void Scene1::setUniforms()
{
    mXYZ->setTransformMatrixUniform(mPlainTransformMatrixUniform);
    mGrid->setTransformMatrixUniform(mPlainTransformMatrixUniform);
    mTriangleSurface->setTransformMatrixUniform(mPhongTransformMatrixUniform);

    mContourLine->setTransformMatrixUniform(mPlainTransformMatrixUniform);
    mBCLightMover->setTransformMatrixUniform(mPlainTransformMatrixUniform);
    mSimulationObject->setTransformMatrixUniform(mPlainTransformMatrixUniform);

    //LIGHT
    mLight->setTransformMatrixUniform(mPhongTransformMatrixUniform);
    mLight->setIntensityUniform(mLightIntensityUniform);
    mLight->setLightColorUniform(mLightColorUniform);
    mLight->setLightPositionUniform(mLightPositionUniform);
}

void Scene1::listObjects()
{
    mObjects.push_back(mXYZ);
    mObjects.push_back(mGrid);
    mObjects.push_back(mTriangleSurface);
    mObjects.push_back(mLight);
    mObjects.push_back(mBCLightMover);
    mObjects.push_back(mSimulationObject);
    mObjects.push_back(mContourLine);
}

void Scene1::setTransformations()
{
    mSimulationObject->scale(0.2f);
}


