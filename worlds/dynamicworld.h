#ifndef DYNAMICWORLD_H
#define DYNAMICWORLD_H

#include "physicsengine.h"
class SimulationWorld;

//Is not inheriting from worldclass
class DynamicWorld
{
    SimulationWorld *mSimulationWorld;
    PhysicsEngine mPhysicsEngine;

    //input = entity, output = collisioncomponent
    std::map<int, CollisionComponent> mCollisionComponents;
    //input = entity, output = transformcomponent
    std::map<int, TransformComponent> mTransformComponents;


public:
    DynamicWorld();


    void Tick(float deltaTime);
    void addComponent(Component *comp, int ent);
    Component *getComponent(int ent, EComponentType type);
    Component &getComponentData(int ent, EComponentType type);

private:
    void applyPhysics(float deltaTime);
    void checkCollisions();
};

#endif // DYNAMICWORLD_H
