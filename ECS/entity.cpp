#include "entity.h"

#include "components.h"


void Entity::attachComponent(Component *newComponent)
{
    mComponentMap[newComponent->type()] = mComponents.size();
    mComponents.push_back(newComponent);
}

Component *Entity::getComponent(EComponentType type) const
{

    if(mComponentMap.find(type) != mComponentMap.end())
    {
//        switch(type)
//        {
//            case EComponentType::mesh:
//            std::cout << "mesh indexcount : " << static_cast<MeshComponent*>(mComponents.at(mComponentMap.at(type)))->indexCount << std::endl;
//        }

        return mComponents.at(mComponentMap.at(type));
    }
    else
    {
        return nullptr;
    }
}

std::vector<Component *> Entity::getAllComponents() const
{
    return mComponents;
}
