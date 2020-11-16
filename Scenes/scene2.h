#ifndef SCENE2_H
#define SCENE2_H

#include "scene.h"

//Folder task part 2

class Scene2 : public Scene
{
protected:

public:
    Scene2();

    virtual void draw(float deltaTime) override;

    virtual void listObjects() override;

protected:
    unsigned int mLerpVectorIndex = 0;

};

#endif // SCENE2_H
