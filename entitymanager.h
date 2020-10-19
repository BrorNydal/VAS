#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "entity.h"
#include "components.h"

/**
    Should be renamed as resourcesmanager has similar name but no similar functionality or data.
 **/
class EntityManager
{
private:
    //the entities
    std::vector<Entity> mEntities;

    //maps for indices

    //transform
    std::map<int, TransformComponent> mTransformMap;

    //camera
    unsigned int mCameraIndex = 0;
    std::map<int, CameraComponent> mCameraMap;

    //player input
    unsigned int mPlayerControllerIndex = 0;
    InputComponent mPlayerInputComponent;
    //std::map<int, InputComponent> mInputMap;

    //index map to resourcemaager
    std::map<int, int> mMeshMap;
    std::map<int, int> mTextureMap;
    std::map<int, int> mTerrainMap;
    std::map<int, int> mGuiMap;

public:
    EntityManager();

    /**
     * Creates a new entity.
     * @return returns index to entity
     */
    Entity *newEntity();

    /**
     * Creates a new entity and attaches mesh to it.
     * @param mesh to attach to the new entity
     * @return returns index to entity
     */
//    int newEntityAndBindToMesh(const std::string &mesh);

    /**
     * Attaches a component to an entity. (Editor only)
     * @param component to attach to entity
     * @param entity to attach component to
     */
    void attachComponentToEntity(Component *component, int entity);

    /**
     * Attach a resource to an entity.
     * @param ent is the entity using the resource
     * @param res is the resource attaching to entity from resourcemanager
     * @param type is the type of resource attaching
     */
    void attachEntityToResource(int ent, int res, EComponentType type);

    //getters
    TransformComponent              &getTransformComponent(int entity);
    CameraComponent                 &getCameraComponent(int entity);
    std::vector<Entity>             &getEntities();
    const std::map<int, int>        &getMeshMap() const;
    const std::map<int, int>        &getComponentIndexMap(EComponentType type);
    int getMeshMapKey(int entity) const;
    int getTextureMapKey(int entity) const;
    int getGuiMapKey(int entity) const;
    std::map<int, CameraComponent>  &getCameraMap();
    InputComponent                  &getPlayerInputComponent();
    unsigned int getPlayerControllerIndex() const;

private://functions

    GuiComponent &newGuiComponent(GuiComponent gui);

};

#endif // ENTITYMANAGER_H
