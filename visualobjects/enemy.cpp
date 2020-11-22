#include "enemy.h"

#include "item.h"

#include "mymathfunctions.h"


Enemy::Enemy(std::string file, BSplineCurve *path)
    :   OBJ(file), mPath(path)
{
    mPhysicsProperties.physic = false;
}


PhysicsProperties &Enemy::getPhysicsProperties()
{
    return mPhysicsProperties;
}

void Enemy::draw(Shader &shader)
{
    OBJ::draw(shader);


}

void Enemy::update()
{
    QVector3D loc = getLocationOnPath(mPathIndex, mPathDelta);
    getTransform().location.setX(loc.x());
    getTransform().location.setY(loc.y());

    const float velocity = 0.1f;
    mPathDelta += velocity;

    if(mPathDelta >= 1.f)
    {
        mPathIndex++;
        mPathDelta = 0.f;
    }

    qDebug() << mPathIndex;
    qDebug() << mPathDelta;
}

void Enemy::notify(NPC_Notification notification)
{
    switch(notification)
    {
        case NPC_Notification::ITEM_TAKEN:

        break;
        case NPC_Notification::PLAYER_DETECTED:

        break;
        case NPC_Notification::ENDPOINT_ARRIVED:

        break;
        case NPC_Notification::OBSTACLE_DETECTED:

        break;

        default:
        break;
    }
}

void Enemy::setItemsGuarding(std::vector<Item *> items)
{
    mGuardingItems = items;
}

void Enemy::findPath()
{
    //sort items based on which is closest to current location

    unsigned int n = mGuardingItems.size();
    float to[n];
    unsigned int indecies[n];

    for(unsigned int i = 0; i < n; i++)
    {
        indecies[i] = i;
        to[i] = (mGuardingItems.at(i)->getLocation() - getLocation()).length();
    }

    MySortFunctions::selectionSort(to, indecies, n);

    //create a bspline based on sorted indices

    std::vector<QVector3D> cp; //controll points
    const unsigned int d = 2; //degree

    //for each item
    for(unsigned int i = 0; i < n; i++)
    {
        //push back item at(soted index list at(i))
        qDebug() << "index :" << indecies[i];
        cp.push_back(mGuardingItems.at(indecies[i])->getLocation());
        qDebug() << "SUC";
    }

    const unsigned int keyvalue = n + d + 1;
    std::vector<float> k; //knots
    unsigned int kvalue = 0;

    //d + 1 equal knots at beginning and end
    for(unsigned int i = 0; i <= d; i++)
        k.push_back(kvalue);

    kvalue++;

    //knots between the beginning and end
    for(unsigned int i = 0; i < keyvalue - 2*(d+1); i++)
    {
        k.push_back(kvalue);
        kvalue++;
    }

    //d + 1 at end
    for(unsigned int i = 0; i <= d; i++)
        k.push_back(kvalue);

    mPath->setNewValues(k, cp, d);
}

QVector3D Enemy::getLocationOnPath(unsigned int &index, float delta)
{
    bool end;
    QVector3D result = mPath->getLocationOnSpline(index, delta, end);

    if(end)
        notify(NPC_Notification::ENDPOINT_ARRIVED);

    return result;
}
