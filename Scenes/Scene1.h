#ifndef TASK_5_2_10_H
#define TASK_5_2_10_H

#include "Scenes/Scene.h"

class Scene1 : public Scene
{
protected:
    class Terrain *mTerrain;
    class ObjFile *mPlayerObject;

public:
    Scene1();

    virtual void draw() override;

    virtual void setUniforms() override;
    virtual void listObjects() override;

protected:
    virtual void setTransformations() override;
};

#endif // TASK_5_2_10_H
