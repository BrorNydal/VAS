#ifndef WORLD_H
#define WORLD_H

#include "entitymanager.h"


/**
 * Divide into three:
 * EditorWorld.
 * SimulationWorld.
 * DynamicWorld.
 */

class World
{
protected:
    std::string mTag;
    EntityManager mEntityManager;    

public:
    World();

    /**
     *
     */
    void initialize();
    virtual void tick(float deltaTime);

    EntityManager *getEntityManager();    

    /**
     * Spawns an object and attaches a meshcomponent to it.
     * @param mesh to attach to object
     */
    void spawnObject(const std::string &mesh);
    /**
     * Spawns an object and attaches a meshcomponent to it.
     * @param mesh to attach to object.
     * @param success returns true if object is successfully spawned
     */
    void spawnObject(const std::string &mesh, bool &success);
    /**
     * Spawns a player object.
     * Spawns an object intended to be controlled by the player,
     * and attaches a mesh to it.
     * @param mesh to attach to object
     */
    void spawnPlayerObject(const std::string &mesh);
    /**
     * Spawns a player object.
     * Spawns an object intended to be controlled by the player,
     * and attaches a mesh to it.
     * @param mesh to attach to object
     * @param success returns true if player object is successfully spawned
     */
    void spawnPlayerObject(const std::string &mesh, bool &success);

    void setTag(const std::string &tag);

    const std::string &getTag() const;
};

#endif // WORLD_H
