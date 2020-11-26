#ifndef SCENE2_H
#define SCENE2_H

#include "scene.h"

//KandidatNr: 6003
//Folder task part 2
//read las file and import height data
//Using height data create a terrain
//Simulate one or more rolling balls on the terrain

class Scene2 : public Scene
{
protected:
    std::vector<RollingBall*> mPhysicsObjects;

public:
    Scene2(TriangleSurface *ts = nullptr);

    virtual void draw(float deltaTime) override;

    virtual void listObjects() override;

protected:

};

#endif // SCENE2_H
