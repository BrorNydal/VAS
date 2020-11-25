#ifndef SCENE3_H
#define SCENE3_H

#include "scene.h"
#include "enemy.h"
#include "item.h"

class Scene3 : public Scene
{
private:
    std::vector<Enemy*> mEnemies;
    std::vector<RollingBall*> mPhysicsObjects;
    std::vector<Item*> mItems;
    std::map<RollingBall*, QVector3D> mDefaultPositions;


public:
    Scene3(IndexedTriangleSurface *ts = nullptr);

    virtual void draw(float deltaTime) override;
    virtual void reset() override;
protected:

    virtual void listObjects() override;
};

#endif // SCENE3_H
