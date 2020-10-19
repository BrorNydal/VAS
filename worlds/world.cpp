#include "world.h"

//systems
#include "rendersystem.h"
#include "inputsystem.h"
#include "guisystem.h"

#include "ECS/coreengine.h"

//audio
#include "soundmanager.h"
#include "wave.h"

#include "mainwindow.h"

World::World()
    :   mTag("World"), mEntityManager()
{
    initialize();
}

void World::initialize()
{
    Entity *terrain = mEntityManager.newEntity();
    int terrainID = terrain->ID;

    TerrainComponent *tc = static_cast<TerrainComponent*>(ResourceManager::getComponent(EComponentType::terrain, "perlinnoise.png"));

    mEntityManager.attachComponentToEntity(tc, terrainID);
}

void World::tick(float deltaTime)
{

}

EntityManager *World::getEntityManager()
{
    return &mEntityManager;
}

void World::spawnObject(const std::string &mesh)
{
    SoundManager::get()->init();

    int mci = ResourceManager::getComponentIndex(EComponentType::mesh, mesh);
    bool meshExists = mci >= 0 ? true : false;

    //If mesh doesnt exist
    if(meshExists)
    {
        static float tempMove = 0.f;

        Entity *ent = mEntityManager.newEntity();

        assert(ent != nullptr);

        int objectID = ent->ID;

        TransformComponent *tc = new TransformComponent();
        mEntityManager.attachComponentToEntity(tc, objectID);

        MeshComponent *mc = ResourceManager::getMesh(mci);
        mEntityManager.attachComponentToEntity(mc, objectID);

        SphereCollider *scc = new SphereCollider(2.f);
        mEntityManager.attachComponentToEntity(scc, objectID);

        tc->rotation = vec3(0,0,90.f);
        tc->location += vec3(tempMove, tempMove, 0.f);
        tempMove += 1.f;

        // play sound

        Wave* spawnSound;

        spawnSound = SoundManager::createSource(
                    "successful spawn sound", vec3(0.0f, 0.0f, 0.0f),
                    "spawnSuccess", false, 1.f);

        CameraComponent *cc = &Engine::EditorCamera();

        if(cc != nullptr)
        {
            SoundManager::get()->updateListener(cc->worldPosition + cc->relativePosition, vec3(0,0,0), cc->forward, cc->up);
            std::cout << "PLay sound" << std::endl;
        }
        else
            std::cout << "Camera nullptr . " << std::endl;

        spawnSound->play();

        Engine::selectEntity(ent);
        Engine::getMainWindow()->updateSceneTree();
    }
    //if mesh exist
    else
    {
        Wave* spawnSound;

        spawnSound = SoundManager::createSource(
                    "fail spawn sound", vec3(0.0f, 0.0f, 0.0f),
                    "spawnFail", false, 1.f);

        CameraComponent *cc = nullptr;

        // temporary solution to getting camera

        for(auto cam = mEntityManager.getCameraMap().begin(); cam != mEntityManager.getCameraMap().end(); cam++)
        {
            cc = &cam->second;
            break;
        }

        if(cc != nullptr)
        {
            SoundManager::get()->updateListener(cc->worldPosition + cc->relativePosition, vec3(0,0,0), cc->forward, cc->up);
            std::cout << "PLay sound" << std::endl;
        }
        else
            std::cout << "Camera nullptr . " << std::endl;

        spawnSound->play();
    }

}

void World::spawnObject(const std::string &mesh, bool &success)
{
    //spawn object

//    int object = mEntityManager.newEntity();
//    mEntityManager.attachMeshAndTransformToEntity(mesh, TransformComponent(), object);
//    MeshComponent &mcPlayer = ResourceManager::getMesh(mEntityManager.getMeshMapKey(object));
//    mcPlayer.texture->setFile("back.png");
//    mcPlayer.texture->readFile();
    //mEntityManager.attachTextureToEntity("CartoonLava.jpg", player);  <---- doesent work properly yet...

    int object = mEntityManager.newEntity()->ID;
    TransformComponent *tc = new TransformComponent();
    mEntityManager.attachComponentToEntity(tc, object);
    mEntityManager.attachComponentToEntity(ResourceManager::getMesh(mesh), object);

    // play sound

    SoundManager::get()->init();

    Wave* spawnSound;

    spawnSound = SoundManager::createSource(
                "spawn sound", vec3(0.0f, 0.0f, 0.0f),
                "spawnSuccess", false, 1.f);

    CameraComponent *cc = nullptr;

    // temporary solution to getting camera

    for(auto cam = mEntityManager.getCameraMap().begin(); cam != mEntityManager.getCameraMap().end(); cam++)
    {
        cc = &cam->second;
        break;
    }

    if(cc != nullptr)
    {
        SoundManager::get()->updateListener(cc->worldPosition + cc->relativePosition, vec3(0,0,0), cc->forward, cc->up);
        std::cout << "PLay sound" << std::endl;
    }
    else
        std::cout << "Camera nullptr . " << std::endl;

    spawnSound->play();
}

void World::spawnPlayerObject(const std::string &mesh)
{
//    int object = mEntityManager.newEntity()->ID;
//    mEntityManager.attachMeshAndTransformToEntity(mesh, TransformComponent(), object);
//    MeshComponent *mcPlayer = ResourceManager::getMesh(mEntityManager.getMeshMapKey(object));
//    mcPlayer->texture->setFile("back.png");
//    mcPlayer->texture->readFile();
//    //mEntityManager.attachTextureToEntity("CartoonLava.jpg", player);  <---- doesent work properly yet...
//    mEntityManager.attachCameraToEntity(CameraComponent(), object);
//    mEntityManager.attachInputToEntity(InputComponent(), object);
}

void World::spawnPlayerObject(const std::string &mesh, bool &success)
{
//    int object = mEntityManager.newEntity()->ID;
//    mEntityManager.attachMeshAndTransformToEntity(mesh, TransformComponent(), object);
//    MeshComponent *mcPlayer = ResourceManager::getMesh(mEntityManager.getMeshMapKey(object));
//    mcPlayer->texture->setFile("back.png");
//    mcPlayer->texture->readFile();
//    //mEntityManager.attachTextureToEntity("CartoonLava.jpg", player);  <---- doesent work properly yet...
//    mEntityManager.attachCameraToEntity(CameraComponent(), object);
//    mEntityManager.attachInputToEntity(InputComponent(), object);
}

void World::setTag(const std::string &tag)
{
    mTag = tag;
}

const std::string &World::getTag() const
{
    return mTag;
}
