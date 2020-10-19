#include "simulationworld.h"

#include "resourcemanager.h"
#include "rendersystem.h"

SimulationWorld::SimulationWorld()
{

}

void SimulationWorld::tick(float deltaTime)
{
    World::tick(deltaTime);

    RenderSystem::renderSimulation();
    mDynamicWorld.Tick(deltaTime);
}

void SimulationWorld::addEntity(Entity *ent)
{
    Entity *newEnt = mEntityManager.newEntity();
    int entID = newEnt->ID;

    if(ent->getComponent(EComponentType::transform) != nullptr)
        mDynamicWorld.addComponent(ent->getComponent(EComponentType::transform), entID);

    if(ent->getComponent(EComponentType::collider) != nullptr)
        mDynamicWorld.addComponent(ent->getComponent(EComponentType::collider), entID);

   if(ent->getComponent(EComponentType::terrain) != nullptr)
       mEntityManager.attachEntityToResource(entID,
                                             ResourceManager::getComponentIndex(EComponentType::terrain,
                                                                                static_cast<TerrainComponent*>(ent->getComponent(EComponentType::terrain))->tag),
                                            EComponentType::terrain);

   if(ent->getComponent(EComponentType::mesh) != nullptr)
       mEntityManager.attachEntityToResource(entID,
                                             ResourceManager::getComponentIndex(EComponentType::mesh,
                                                                                static_cast<TerrainComponent*>(ent->getComponent(EComponentType::mesh))->tag),
                                             EComponentType::mesh);
}

Component &SimulationWorld::getPhysicsComponent(int ent, EComponentType type)
{
    return mDynamicWorld.getComponentData(ent, type);
}
