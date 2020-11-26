#ifndef TASK_5_2_10_H
#define TASK_5_2_10_H

#include "scene.h"

//KandidatNr: 6003
//Folder task part 1
//Create a terrain of triangles that is easy to calculate
//Simulate a ball rolling from one to another

class Scene1 : public Scene
{
private:


public:
    Scene1(TriangleSurface *ts = nullptr);

    virtual void draw(float deltaTime) override;
    virtual void listObjects() override;
};

#endif // TASK_5_2_10_H
