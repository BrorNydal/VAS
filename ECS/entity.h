#ifndef ENTITY_H
#define ENTITY_H

#include "enumclasses.h"

#include <vector>
#include <map>
class Component;

struct Entity{
    /**
     * Entity constructor.
     * @param id must be unique for each entity
     */
    Entity(int id) : ID(id) {}
    const int ID;    

    operator int() const {return ID;}

    /**
     * Activates entity.
     * All activated entities will run in systems,
     * this may decrease the framerate.
     */
    void activate(){ active = true; }
    /**
     * Deactivates entity.
     * All Deactivated entities will not run in systems,
     * this may increase the framerate.
     */
    void deactivate(){ active = false; }
    /**
     * Is entity active.
     * Active entities will run in systems,
     * deactivated entities will not
     * @return returns true if active
     */
    bool isActive() const {return active;}
    /**
     * Attaches a new component to this entity (Editor Only).
     * Attaches a pointer component to entity,
     * will only be utilized in editor and is
     * not utilized when simulationg the game.
     * @param newComponent is the component to attach to entity
     */
    void attachComponent(Component *newComponent);
    /**
     * Gets a component of given type (Editor Only).
     * @param type of component
     * @return returns a the component of given type if any, else returns nullptr
     */
    Component *getComponent(EComponentType type) const;

    std::vector<Component*> getAllComponents() const;

private:
    bool active = true;

    std::map<EComponentType, int> mComponentMap;
    std::vector<Component*> mComponents;
};

#endif // ENTITY_H
