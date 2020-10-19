#ifndef EDITORWORLD_H
#define EDITORWORLD_H

#include "world.h"
#include "entity.h"

class EditorWorld : public World
{
private:
    std::vector<Entity> mEditorEntities;

public:
    EditorWorld();

    virtual void tick(float deltaTime) override;

private:
    void createEditorEntities();
};

#endif // EDITORWORLD_H
