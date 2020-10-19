#include "editorworld.h"

#include "rendersystem.h"
#include "inputsystem.h"

EditorWorld::EditorWorld()
{
    createEditorEntities();
}

void EditorWorld::tick(float deltaTime)
{
    World::tick(deltaTime);

    RenderSystem::renderEditor();
    InputSystem::run();
}

void EditorWorld::createEditorEntities()
{

}
