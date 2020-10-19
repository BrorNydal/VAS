#include "entitymanager.h"

#include "resourcemanager.h"

#include "vertex.h"

EntityManager::EntityManager()
{
    //initializeOpenGLFunctions();
}

Entity *EntityManager::newEntity()
{
    unsigned int i = mEntities.size();
    mEntities.push_back(Entity(i));

    return &mEntities.at(i);
}

void EntityManager::attachComponentToEntity(Component *component, int entity)
{
    if(static_cast<int>(mEntities.size()) > entity)
    {
        mEntities.at(entity).attachComponent(component);
    }
}

void EntityManager::attachEntityToResource(int ent, int res, EComponentType type)
{
    switch(type)
    {
    case EComponentType::mesh:
        mMeshMap[ent] = res;
        break;

    case EComponentType::terrain:
        mTerrainMap[ent] = res;
        break;

    case EComponentType::texture:
        mTextureMap[ent] = res;
        break;
    }
}

std::vector<Entity> &EntityManager::getEntities()
{
    return mEntities;
}

const std::map<int, int> &EntityManager::getMeshMap() const
{
    return mMeshMap;
}

const std::map<int, int> &EntityManager::getComponentIndexMap(EComponentType type)
{
    switch(type)
    {
    case EComponentType::mesh:
        return mMeshMap;
        break;

    case EComponentType::terrain:
        return mTerrainMap;
        break;

    case EComponentType::texture:

        break;

    default:
        break;
    }

    //to avoid warnings, but type should always be specified
    //the function should not reach this point
    return mMeshMap;
}

int EntityManager::getMeshMapKey(int entity) const
{
    return mMeshMap.at(entity);
}

int EntityManager::getTextureMapKey(int entity) const
{
    return mTextureMap.at(entity);
}

int EntityManager::getGuiMapKey(int entity) const
{
    return mGuiMap.at(entity);
}

std::map<int, CameraComponent> &EntityManager::getCameraMap()
{
    return mCameraMap;
}

InputComponent &EntityManager::getPlayerInputComponent()
{
    return mPlayerInputComponent;
}

unsigned int EntityManager::getPlayerControllerIndex() const
{
    return mPlayerControllerIndex;
}

GuiComponent &EntityManager::newGuiComponent(GuiComponent gui)
{
    return gui;
}

