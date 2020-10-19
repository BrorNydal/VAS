#include "dynamicworld.h"

DynamicWorld::DynamicWorld()
{

}

void DynamicWorld::Tick(float deltaTime)
{
    applyPhysics(deltaTime);
}

void DynamicWorld::addComponent(Component *comp, int ent)
{
    switch(comp->type())
    {
        case EComponentType::transform:
        switch(static_cast<TransformComponent*>(comp)->movementLimitation)
        {
        case EMovementLimitType::fixed:
        mTransformComponents[ent] = *static_cast<TransformComponent*>(comp);
        break;

        case EMovementLimitType::dynaimc:
        mTransformComponents[ent] = *static_cast<TransformComponent*>(comp);
        break;

        case EMovementLimitType::controlled:
        mTransformComponents[ent] = *static_cast<TransformComponent*>(comp);
        break;

        default:
            qDebug() << "(dynamicworld) invalid component.";
            break;
        }

        //switch(collirdertype)
    case EComponentType::collider:
        mCollisionComponents[ent] = *static_cast<CollisionComponent*>(comp);
        break;

    default:
        break;
    }
}

Component *DynamicWorld::getComponent(int ent, EComponentType type)
{
    switch(type)
    {
    case EComponentType::transform:
        return &mTransformComponents[ent];
        break;

    case EComponentType::collider:
        return &mCollisionComponents[ent];
        break;
    }
}

Component &DynamicWorld::getComponentData(int ent, EComponentType type)
{
    switch(type)
    {
    case EComponentType::transform:
        return mTransformComponents[ent];
        break;

    case EComponentType::collider:
        return mCollisionComponents[ent];
        break;
    }
}

void DynamicWorld::applyPhysics(float deltaTime)
{
    for(auto map = mTransformComponents.begin(); map != mTransformComponents.end(); map++)
    {
        if((*map).second.movementLimitation != EMovementLimitType::fixed)
            mPhysicsEngine.applyPhysics((*map).second, deltaTime);
    }
}

void DynamicWorld::checkCollisions()
{
    //do spatial partitioning
    //sort fixed, dynamic and controlled entities
    //check each possible collision
}
