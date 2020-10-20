#ifndef SCENE2_H
#define SCENE2_H

#include "Scenes/Scene.h"

//TASK 5.2.11

class Scene2 : public Scene
{
protected:
    class VisualObject           *mXYZ;
    class VisualObject           *mGrid;
    class IndexedTriangleSurface *mTriangleSurface;
    class Light                  *mLight;
    class BezierCurve            *mBCLightMover;
    class VisualObject           *mSimulationObject;
    class ContourLine            *mContourLine;

    //Which index on beziercurve the objects on and how many iterations we move per frame
    unsigned int mLightMoverIndex   = 0;
    const unsigned int mLightSpeed        = 1;

    //Which index on beziercurve the objects on and how many iterations we move per frame
    unsigned int mSimMoverIndex   = 0;
    const unsigned int mSimSpeed        = 1;

public:
    Scene2();

    virtual void draw() override;

    virtual void setUniforms() override;
    virtual void listObjects() override;

protected:
    virtual void setTransformations() override;
    unsigned int mLerpVectorIndex = 0;

    //Cuts the desired path into 3 pieces, creating 3 beziercureves
    //We move the simulation object along these beziercurves
    void initializemBCSimulationObjectPath();
    std::vector<BezierCurve*> mBCSimulationObjectPath;
    unsigned int mPathFraction = 3;
};

#endif // SCENE2_H
