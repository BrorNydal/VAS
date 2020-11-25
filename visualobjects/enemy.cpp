#include "enemy.h"

#include "item.h"

#include "mymathfunctions.h"


Enemy::Enemy(BSplineCurve *path, RollingBall *player)
    :   OBJ("enemy.obj"), mPath(path), mPlayer(player)
{
    mPhysicsProperties.physic = false;
}


PhysicsProperties &Enemy::getPhysicsProperties()
{
    return mPhysicsProperties;
}

void Enemy::draw(Shader &shader)
{
    shader.uniform3f("color", 1.f, 1.f, 0.f);
    OBJ::draw(shader);
    mPath->draw(shader);
}

void Enemy::update()
{

    //FSM
    switch(getState())
    {
    case NPC_State::PATROL:
        patrol();
        break;

    case NPC_State::CHASE:
        chase();
        break;

    case NPC_State::LEARN:
        learn();
        break;
    }
}

void Enemy::notify(NPC_Notification notification)
{
    switch(notification)
    {
        case NPC_Notification::ITEM_TAKEN:
        setState(NPC_State::LEARN);
        break;
        case NPC_Notification::PLAYER_DETECTED:
        setState(NPC_State::CHASE);
        break;
        case NPC_Notification::ENDPOINT_ARRIVED:
        setState(NPC_State::LEARN);
        qDebug() << "ENDPOINT";
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

void Enemy::removeItem(Item *item)
{
    for(auto it = mGuardingItems.begin(); it != mGuardingItems.end(); it++)
    {
        if((*it) == item)
        {
            mGuardingItems.erase(it);
            break;
        }
    }

    mGuardingItems.shrink_to_fit();

    notify(NPC_Notification::ITEM_TAKEN);
}

// findPath() does this in following order:
/*
 * Sort items by shortest to longest distance
 * Create new vector for controlpoints
 * Push back enemy location, and then the sorted items to control points
 * Then i created an algorithm to find fitting knot numbers which match -> n + d + 1
 * with d + 1 equal nots on each end
 * After this we set the new values for the bspline (mPath) and create it.
 */
void Enemy::findPath()
{
    //degree / enemy movement pattern
    const unsigned int d = 2;

    //controll points
    std::vector<QVector3D> cp;

    //For each active item, find location in mGuardingItems vector
    std::vector<unsigned int> id;
    for(unsigned int i = 0; i < mGuardingItems.size(); i++)
    {
        if(mGuardingItems[i]->IsActive())
            id.push_back(i);
    }

    //number of controlpoints are for now equal to number of active items
    unsigned int n = id.size();

    //special case: Enemy should encircle item
    if(n == 1)
    {
        cp.push_back(QVector3D(getLocation().x(), getLocation().y(), 0.f));

        const float radius = 10.f;
        cp.push_back(mGuardingItems[id[0]]->getLocation() + QVector3D(radius, 0.f, 0.f));
        cp.push_back(mGuardingItems[id[0]]->getLocation() + QVector3D(0.f, radius, 0.f));
        cp.push_back(mGuardingItems[id[0]]->getLocation() + QVector3D(-radius, 0.f, 0.f));
        cp.push_back(mGuardingItems[id[0]]->getLocation() + QVector3D(0.f, -radius, 0.f));

        n = 5;
    }

    //special case: Find random point on map maybe? Or maybe find an item to encircle? (If time, else stand still)
    else if(n == 0)
    {
        qDebug() << "TRAVEL";
        cp.push_back(QVector3D(getLocation().x(), getLocation().y(), 0.f));

    }

    //Using selection-sort: sort items based on which is closest to current enemy location
    else
    {
        //selection-sort function takes an array of template type
        float to[n];

        //can also take in an index array that is sorted paralell to template type array
        unsigned int indecies[n];

        //for each item, place it into array
        for(unsigned int i = 0; i < n; i++)
        {
            indecies[i] = id[i];
            to[i] = (mGuardingItems.at(id[i])->getLocation() - getLocation()).length();
        }

        //method of sorting: sorts arrays given size n
        MySortFunctions::selectionSort(to, indecies, n);

        //create a bspline based on sorted indices

        //enemy current location becomes a controlpoint
        cp.push_back(QVector3D(getLocation().x(), getLocation().y(), 0.f)); n++;

        //for each item, hence n-1 as enemy location occupies the first element
        for(unsigned int i = 0; i < n-1; i++)
            cp.push_back(mGuardingItems.at(indecies[i])->getLocation());

    }

    //qDebug() << "n :" << n;
    const unsigned int keyvalue = n + d + 1;
    std::vector<float> k; //knots
    float kvalue = 0.f;

    //d + 1 equal knots at beginning and end
    for(unsigned int i = 0; i <= d; i++)
        k.push_back(kvalue);

    kvalue+= 1.f;

    //knots between the beginning and end
    for(unsigned int i = 0; i < keyvalue - 2*(d+1); i++)
    {
        k.push_back(kvalue);
        kvalue+=1.f;
    }

    //d + 1 at end
    for(unsigned int i = 0; i <= d; i++)
        k.push_back(kvalue);

    //for(auto cppp : cp)
    //    qDebug() << "cp"  <<cppp;
    //for(auto kntt : k)
    //    qDebug() << "kt" << kntt;

    qDebug() << "PATH ALGO FIN";
    mPath->setNewValues(k, cp, d);
    qDebug() << "NEW VALUES SET";
}

void Enemy::patrol()
{
    if(mPath == nullptr)
    {
        qDebug() << "path nullptr, creating path.";
        mPath = new BSplineCurve();
        notify(NPC_Notification::ENDPOINT_ARRIVED);
    }

    bool end;
    QVector3D loc = getLocationOnPath(mPathIndex, mPathDelta, end);

    if(end)
    {
        notify(NPC_Notification::ENDPOINT_ARRIVED);
        return;
    }

    getTransform().location.setX(loc.x());
    getTransform().location.setY(loc.y());    

    float d = mVelocity / mPath->lengthOfCurrentStep(mPathIndex);
    mPathDelta += d;

    if(mPathDelta >= 1.f)
    {
        mPathIndex++;
        mPathDelta = 0.f;
    }
}

void Enemy::chase()
{
    if(mPlayer != nullptr)
    {
        //chase player

        QVector3D to = mPlayer->getLocation() - getLocation();

        if(to.length() > mDetectionRadius)
            setState(NPC_State::LEARN);

        to.normalize();

        setLocation(getLocation() + to * mVelocity);
        to *= mVelocity;
    }
}

void Enemy::learn()
{
    mPathIndex = 0;
    mPathDelta = 0.f;
    findPath();
    setState(NPC_State::PATROL);
}

void Enemy::setState(NPC_State state)
{
    mState = state;
}

NPC_State Enemy::getState() const
{
    return mState;
}

QVector3D Enemy::getLocationOnPath(unsigned int &index, float delta, bool &end)
{
    QVector3D result = mPath->getLocationOnSpline(index, delta, end);
    return result;
}
