#ifndef SCENE2_H
#define SCENE2_H

#include "Scenes/Scene.h"

class Scene2 : public Scene
{
protected:
    class VisualObject           *mXYZ;
    class VisualObject           *mGrid;
    class Light                  *mLight;

public:
    Scene2();

    virtual void draw() override;

    virtual void setUniforms() override;
    virtual void listObjects() override;

protected:
    virtual void setTransformations() override;
};

#endif // SCENE2_H
