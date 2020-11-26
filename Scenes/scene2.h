#ifndef SCENE2_H
#define SCENE2_H

#include "scene.h"

//Folder task part 2

class Scene2 : public Scene
{
protected:
    unsigned int mLerpVectorIndex = 0;
public:
    Scene2(IndexedTriangleSurface *ts = nullptr);

    virtual void draw(float deltaTime) override;

    virtual void listObjects() override;

protected:

};

#endif // SCENE2_H
