#ifndef ENEMY_H
#define ENEMY_H

#include "obj.h"

#include "bspline.h"
#include "rollingball.h"

class Item;

/*
 * Following FSM model the enemy has a given number of states.
 * I use these enum classes to specify which state it's in,
 * and notify it of what is happening.
 * Based on notifications the enemy can chose to transition into
 * another state or not.
 */
enum class NPC_State {
    PATROL, //Follows a path that's close to each of the items.
    LEARN,  //Based on new and existing informartion it creates a new path.
    CHASE   //Chases the player when the player is close enough.
};

//Note: Explain transition to chase when player detected in report.
enum class NPC_Notification {
    ENDPOINT_ARRIVED,   //transition->learn
    ITEM_TAKEN,         //transition->learn
    OBSTACLE_DETECTED,  //transition->learn
    PLAYER_DETECTED     //transition->chase
};

enum class NPC_Transition {
    BUILD_NEW_PATH,         //transition->patrol
    REMOVE_ITEM,            //transition->learn
    BUILD_OBSTACLE_PATH,    //transition->patrol
    BUILD_CHASE_PATH        //not in use
};

class Enemy : public OBJ
{    
    PhysicsProperties mPhysicsProperties;

    BSplineCurve *mPath;
    unsigned int mPathIndex{ 0 };
    float mPathDelta{ 0.f };
    float mVelocity{1.f};

    std::vector<Item*> mGuardingItems;
    RollingBall *mPlayer = nullptr;

    float mDetectionRadius{50.f};

    NPC_State mState = NPC_State::PATROL;

public:
    Enemy(BSplineCurve *path = new BSplineCurve(), RollingBall *player = nullptr);

    PhysicsProperties &getPhysicsProperties();

    virtual void draw(Shader &shader) override;
    void update();

    void notify(NPC_Notification notification);

    float getDetectionRadius() const {return mDetectionRadius;}
    void setItemsGuarding(std::vector<Item*> items);
    void removeItem(Item *item);
    void findPath();

    void patrol();
    void chase();
    void learn();

    void setState(NPC_State state);
    NPC_State getState() const;

protected:
    QVector3D getLocationOnPath(unsigned int &index, float delta, bool &end);
};

#endif // ENEMY_H
