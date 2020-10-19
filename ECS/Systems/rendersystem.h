#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "system.h"

#include "components.h"
#include "resourcemanager.h"
#include "entity.h"
#include <vector>
#include <map>

class Shader;
class EntityManager;

/**
    All systems are singleton.
**/
class RenderSystem : public System
{
public:
    RenderSystem();

    static RenderSystem &get();

    /**
     * Renders all active entities in simulation mode.
     * Simulation mode may increase performance as it
     * utilizes datastructures to keep all similar data,
     * and utilizes desireable rendering features.
     */
    static void renderSimulation();

    /**
     * Renders all activated and deactivated entities in editor mode.
     * Editor mode may decrease performance as it uses pointers
     * to components and other non-desireable features.
     */
    static void renderEditor();

private:
    //implementation
    void renderSimulation_impl();
    void renderEditor_impl();

    /**
     * Assigns camera transformation.
     * @param em is the entitymanager fetched from currentworld
     * @return returns index to camera being used
     */
    void cameraLookAt(CameraComponent *cam);
    /**
     * Updates camera.
     * Sets the view matrix of camera to identity, preparing it for another
     * lookat call
     * @param em is the entitymanager fetched from currentworld
     * @param cameraIndex camera to update
     */
    void cameraUpdate(EntityManager *em, int cameraIndex);
};

#endif // RENDERSYSTEM_H
