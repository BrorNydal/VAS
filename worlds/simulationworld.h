#ifndef SIMULATIONWORLD_H
#define SIMULATIONWORLD_H

#include "world.h"
#include "physicsengine.h"
#include "dynamicworld.h"

class SimulationWorld : public World
{
private:
    DynamicWorld mDynamicWorld;

public:
    SimulationWorld();

    virtual void tick(float deltaTime) override;

    /**
     * Adds a new entity to the simulationworld
     * The pointer isn't stored in any container,
     * so it has to be deleted outside this object.
     * @param ent is a pointer to an address stored another place.
     */
    void addEntity(Entity *ent);

    Component &getPhysicsComponent(int ent, EComponentType type);

};

#endif // SIMULATIONWORLD_H
