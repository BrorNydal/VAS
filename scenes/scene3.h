#ifndef SCENE3_H
#define SCENE3_H

#include "scene.h"
#include "enemy.h"
#include "item.h"

//KandidatNr: 6003
//Folder task 3
//Using terrain from task 2
//Create enemies that patrol items
//Enemies find paths using bspline curves
//Player must be a controlable character that can pick up items

class Scene3 : public Scene
{
private:
    std::vector<Enemy*> mEnemies;
    std::vector<RollingBall*> mPhysicsObjects;
    std::vector<Item*> mItems;


public:
    Scene3(TriangleSurface *ts = nullptr);

    virtual void draw(float deltaTime) override;
    virtual void reset() override;
protected:

    virtual void listObjects() override;
};

#endif // SCENE3_H
