#ifndef COREENGINE_H
#define COREENGINE_H

#include "editorworld.h"
#include "simulationworld.h"
#include "resourcemanager.h"
#include "entity.h"
#include "components.h"
#include "Systems/rendersystem.h"
#include "mainwindow.h"

class MainWindow;


/**
    Holds all systems, and the most important variables.
 **/

class Engine
{
private:
    MainWindow *mMainWindow;
    RenderWindow *mRenderWindow;

    ResourceManager mResourceManager;

    CameraComponent mEditorCamera;

    unsigned int tempLastEditorWorld = 0;
    unsigned int mCurrentWorld = 0;
    std::vector<EditorWorld> mEditorWorlds;
    std::vector<SimulationWorld> mSimulationWorlds;    

    Entity *mSelectedEntity = nullptr;

    bool mSimulating = false;

public:
    Engine();

    static Engine &get();

    static void play();
    static void stop();

    /**
     * Initializes the engine.
     */
    static void initialize(MainWindow *mw, RenderWindow *rw);
    /**
     * Runs each frame.
     * @param deltaTime is the time passed each frame
     */
    static void tick(float deltaTime);


    //getters

    /**
     * Gets the current world.
     * @return returns the current world, based on current world index
     */
    static EditorWorld *getEditorWorld();
    static std::vector<EditorWorld> getWorlds();
    static SimulationWorld *getSimulationWorld();
    static void newWorld();

    static void setMainWindow(MainWindow *mainwindow);
    static MainWindow *getMainWindow();
    static CameraComponent &EditorCamera();

    static void selectEntity(Entity *ent);
    static Entity *getSelectedEntity();

private:
    void initialize_impl(MainWindow *mw, RenderWindow *rw);
    void play_impl();
    void stop_impl();
};

#endif // COREENGINE_H
