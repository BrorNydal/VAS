#ifndef ENEMY_H
#define ENEMY_H

#include "obj.h"

#include "bspline.h"
#include "rollingball.h"

class Item;

enum class NPC_State {
    PATROL,
    LEARN,
    CHASE
};

enum class NPC_Notification {
    ENDPOINT_ARRIVED,
    ITEM_TAKEN,
    OBSTACLE_DETECTED,
    PLAYER_DETECTED
};

enum class NPC_Transition {
    BUILD_NEW_PATH,
    REMOVE_ITEM,
    BUILD_OBSTACLE_PATH,
    BUILD_CHASE_PATH
};

class Enemy : public OBJ
{    
    PhysicsProperties mPhysicsProperties;

    BSplineCurve *mPath;
    unsigned int mPathIndex{ 0 };
    float mPathDelta{ 0.f };

    std::vector<Item*> mGuardingItems;
    RollingBall *mPlayer;

    float mDetectionRadius{30.f};

public:
    Enemy(std::string file, BSplineCurve *path = new BSplineCurve());

    PhysicsProperties &getPhysicsProperties();

    virtual void draw(Shader &shader) override;
    void update();

    void notify(NPC_Notification notification);

    float getDetectionRadius() const {return mDetectionRadius;}
    void setItemsGuarding(std::vector<Item*> items);
    void findPath();

protected:
    QVector3D getLocationOnPath(unsigned int &index, float delta);
};

#endif // ENEMY_H
