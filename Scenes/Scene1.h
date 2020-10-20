#ifndef TASK_5_2_10_H
#define TASK_5_2_10_H

#include "Scenes/Scene.h"

//TASK 5.2.10

class Scene1 : public Scene
{
private:
    //List of objects used in this scene and this task (5.2.10)
    class VisualObject      *mXYZ;
    class VisualObject      *mGrid;
    class TriangleSurface   *mTriangleSurface;
    class Light             *mLight;
    class BezierCurve       *mBCLightMover;
    class VisualObject      *mSimulationObject;
    class ContourLine       *mContourLine;

    unsigned int mLightMoverIndex   = 0;
    unsigned int mLightSpeed        = 1;

public:
    Scene1();

    virtual void draw() override;

    virtual void setUniforms() override;
    virtual void listObjects() override;

protected:
    virtual void setTransformations() override;
};

#endif // TASK_5_2_10_H
