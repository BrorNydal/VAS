#include "coreengine.h"
#include "entitymanager.h"
#include "soundmanager.h"

Engine::Engine()
{

}

Engine &Engine::get()
{
    static Engine *mInstance = new Engine();
    return *mInstance;
}

void Engine::play()
{
    get().play_impl();
}

void Engine::stop()
{
    get().stop_impl();
}

void Engine::initialize(MainWindow *mw, RenderWindow *rw)
{
    get().initialize_impl(mw, rw);
}

void Engine::tick(float deltaTime)
{
    if(get().mSimulating == false)
    {
        get().mEditorWorlds.at(get().mCurrentWorld).tick(deltaTime);

        get().mMainWindow->frameUpdate();
    }
    else{
        get().mSimulationWorlds.at(get().mCurrentWorld).tick(deltaTime);
    }
}

EditorWorld *Engine::getEditorWorld()
{
    assert(get().mEditorWorlds.size() > get().mCurrentWorld);

    return &get().mEditorWorlds.at(get().mCurrentWorld);
}

std::vector<EditorWorld> Engine::getWorlds()
{
    return get().mEditorWorlds;
}

SimulationWorld *Engine::getSimulationWorld()
{
    return &get().mSimulationWorlds.at(get().mCurrentWorld);
}

void Engine::setMainWindow(MainWindow *mainwindow)
{
//    get().mMainWindow = mainwindow;
//    get().mMainWindow->updateMeshView();
}

MainWindow *Engine::getMainWindow()
{
    return get().mMainWindow;
}

CameraComponent &Engine::EditorCamera()
{
    return get().mEditorCamera;
}

void Engine::selectEntity(Entity *ent)
{
    get().mSelectedEntity = ent;
}

Entity *Engine::getSelectedEntity()
{
    return get().mSelectedEntity;
}

void Engine::initialize_impl(MainWindow *mw, RenderWindow *rw)
{
    mMainWindow = mw;
    mRenderWindow = rw;

    mResourceManager.initialize();
    mMainWindow->updateMeshView();

    if(mEditorWorlds.size() == 0)
    {
        mEditorWorlds.push_back(EditorWorld());
    }

    //mMainWindow.updateSceneTree();

    qDebug() << "Engine Initialized!";
}

void Engine::play_impl()
{
    tempLastEditorWorld = mCurrentWorld;
    mSimulating = true;

    for(unsigned int i = 0; i < mEditorWorlds.size(); i++)
    {
        std::vector<Entity> ents = mEditorWorlds.at(i).getEntityManager()->getEntities();

        unsigned int simIndex = mSimulationWorlds.size();
        mSimulationWorlds.push_back(SimulationWorld());

        for(auto ent = ents.begin(); ent != ents.end(); ent++)
        {
            mSimulationWorlds.at(simIndex).addEntity(&(*ent));
        }
    }
}

void Engine::stop_impl()
{
    mCurrentWorld = tempLastEditorWorld;
    mSimulating = false;
    mSimulationWorlds.clear();
}
