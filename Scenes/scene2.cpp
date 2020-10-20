#include "Scenes/scene2.h"
#include "MyMath/mymathfunctions.h"

#include "VisualObjects/indexedtrianglesurface.h"
#include "VisualObjects/xyz.h"
#include "VisualObjects/grid.h"
#include "VisualObjects/beziercurve.h"
#include "VisualObjects/octahedronball.h"
#include "VisualObjects/light.h"
#include "VisualObjects/contourline.h"

Scene2::Scene2()
    :   mXYZ(new XYZ()), mGrid(new Grid()), mTriangleSurface(new IndexedTriangleSurface("datasett_5_2_6", "indeks_sett_5_2_7")),
        mLight(new Light()), mBCLightMover(new BezierCurve()), mSimulationObject(new OctahedronBall(1, 3)),
        mContourLine(new ContourLine(mTriangleSurface, 1.5f))
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
    mTriangleSurface->setDisplayObject(mSimulationObject);    

    initializemBCSimulationObjectPath();

    //Camera Position
    mViewMatrix(0, 3) = 0.f;
    mViewMatrix(1, 3) = 0.f;
    mViewMatrix(2, 3) = -8.f;
}

void Scene2::draw()
{
    Scene::draw();

    mLight->setPosition(Vector3D(mBCLightMover->getLocationOnBezierCurve(mLightMoverIndex).x(),
                                 mBCLightMover->getLocationOnBezierCurve(mLightMoverIndex).y(),
                                 mBCLightMover->getLocationOnBezierCurve(mLightMoverIndex).z()));

    mLightMoverIndex += mLightSpeed;

    if(mLightMoverIndex >= mBCLightMover->getBezierCurveVectorSize() - mLightSpeed || mLightMoverIndex <= 0)
        mLightMoverIndex = 0;


    if(mBCSimulationObjectPath[mLerpVectorIndex]->lerpBetweenPositions(mSimulationObject))
    {
        mLerpVectorIndex++;
        if(mLerpVectorIndex == mBCSimulationObjectPath.size())
            mLerpVectorIndex = 0;
    }


    mTriangleSurface->barycentricHeightSearch(mSimulationObject, 0);

//    mSimulationObject->setPosition(Vector3D(mBCSimulationObjectMover->getLocationOnBezierCurve(mSimMoverIndex).x(),
//                                            mBCSimulationObjectMover->getLocationOnBezierCurve(mSimMoverIndex).y(),
//                                            mBCSimulationObjectMover->getLocationOnBezierCurve(mSimMoverIndex).z()));

//    mSimMoverIndex += mSimSpeed;

//    if(mSimMoverIndex >= mBCSimulationObjectMover->getBezierCurveVectorSize() - mSimSpeed || mSimMoverIndex <= 0)
//        mSimMoverIndex = 0;
}

void Scene2::setUniforms()
{
    mXYZ->setTransformMatrixUniform(mPlainTransformMatrixUniform);
    mGrid->setTransformMatrixUniform(mPlainTransformMatrixUniform);
    mTriangleSurface->setTransformMatrixUniform(mPhongTransformMatrixUniform);

    mContourLine->setTransformMatrixUniform(mPlainTransformMatrixUniform);
    mBCLightMover->setTransformMatrixUniform(mPlainTransformMatrixUniform);
    mSimulationObject->setTransformMatrixUniform(mPhongTransformMatrixUniform);

    for(unsigned int i = 0; i < mBCSimulationObjectPath.size(); i++)
        mBCSimulationObjectPath[i]->setTransformMatrixUniform(mPlainTransformMatrixUniform);

    //LIGHT
    mLight->setTransformMatrixUniform(mPhongTransformMatrixUniform);
    mLight->setIntensityUniform(mLightIntensityUniform);
    mLight->setLightColorUniform(mLightColorUniform);
    mLight->setLightPositionUniform(mLightPositionUniform);
}

void Scene2::listObjects()
{
    mObjects.push_back(mXYZ);
    mObjects.push_back(mGrid);
    mObjects.push_back(mTriangleSurface);
    mObjects.push_back(mLight);
    mObjects.push_back(mBCLightMover);
    mObjects.push_back(mSimulationObject);
    mObjects.push_back(mContourLine);

    for(unsigned int i = 0; i < mBCSimulationObjectPath.size(); i++)
        mObjects.push_back(mBCSimulationObjectPath[i]);
}

void Scene2::setTransformations()
{
    mSimulationObject->scale(0.2f);
    for(unsigned int i = 0; i < mBCSimulationObjectPath.size(); i++)
        mBCSimulationObjectPath[i]->setPosition(Vector3D(mBCSimulationObjectPath[i]->getPosition().x(), mBCSimulationObjectPath[i]->getPosition().y(), 0.5f));
}

void Scene2::initializemBCSimulationObjectPath()
{
    unsigned int frac = (mTriangleSurface->mBarycentricSearchTrace.size() - 1) / mPathFraction; //6/3

    unsigned int lastIndex = 0;

    for(unsigned int path = 0; path < mPathFraction; path++)
    {
        mBCSimulationObjectPath.push_back(new BezierCurve());
        std::vector<Vector3D> trace;

        for(unsigned int i = lastIndex; i < ((frac) * (path + 1)) + 1; i++)
        {
            if(i < mTriangleSurface->mBarycentricSearchTrace.size())
                trace.push_back(mTriangleSurface->mBarycentricSearchTrace[i]);
            lastIndex = i;
        }

        mBCSimulationObjectPath[path]->setControlPoints(trace);
    }

    //BezierCurve settings
    for(unsigned int i = 0; i < mBCSimulationObjectPath.size(); i++)
    {
        mBCSimulationObjectPath[i]->setDisplayBezierCurve(true);
        mBCSimulationObjectPath[i]->setIterations(10);
    }
}
