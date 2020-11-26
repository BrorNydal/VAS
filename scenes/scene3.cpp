#include "scene3.h"

#include "obj.h"
#include "structs.h"

Scene3::Scene3(IndexedTriangleSurface *ts)
    :   Scene(ts)
{
    initializeScene();
}

void Scene3::draw(float deltaTime)
{
    Scene::draw(deltaTime);

    if(mPause == true)
        return;

    mBall.getTransform().location.setZ(mTriangleSurface->heightAtLocation(mBall.getLocation().x(), mBall.getLocation().y()) + mBall.getRadius());

    if(mTriangleSurface->getTriangle(mBall.getLocation().x(), mBall.getLocation().y()) != nullptr)
        mPhysicsEngine.ballMovement(deltaTime, mBall.getTransform(), mBall.getPhysicsProperties(), mTriangleSurface->getTriangle(mBall.getLocation().x(), mBall.getLocation().y())->mSurfaceNormal);

    for(auto &enemy : mEnemies)
    {
        enemy->getTransform().location.setZ(mTriangleSurface->heightAtLocation(enemy->getLocation().x(), enemy->getLocation().y()) + 1.f);
    }

    for(auto &physicObject : mPhysicsObjects)
    {
        physicObject->getTransform().location.setZ(mTriangleSurface->heightAtLocation(physicObject->getLocation().x(), physicObject->getLocation().y()) + physicObject->getRadius());

        if(mTriangleSurface->getTriangle(physicObject->getLocation().x(), physicObject->getLocation().y()) != nullptr)
            mPhysicsEngine.ballMovement(deltaTime, physicObject->getTransform(), physicObject->getPhysicsProperties(), mTriangleSurface->getTriangle(physicObject->getLocation().x(), physicObject->getLocation().y())->mSurfaceNormal);
    }

    for(auto &enemy : mEnemies)
        enemy->update();

    CollisionResult collision;    

    for(unsigned int i = 0; i < mPhysicsObjects.size(); i++)
    {
        RollingBall *po1 = mPhysicsObjects[i];

        for(auto &enemy : mEnemies)
        {
            collision = mPhysicsEngine.CheckCollision(Sphere(), po1->getTransform(), Sphere(), enemy->getTransform());

            if(collision.collision)
            {
                mPhysicsEngine.handleCollision(Sphere(), po1->getPhysicsProperties(), po1->getTransform(), Sphere(), enemy->getPhysicsProperties(), enemy->getTransform());
            }
        }

        for(unsigned int k = i + 1; k < mPhysicsObjects.size(); k++)
        {
            RollingBall *po2 = mPhysicsObjects[k];

            if(po2 != po1)
            {
                collision = mPhysicsEngine.CheckCollision(Sphere(), po1->getTransform(), Sphere(), po2->getTransform());

                if(collision.collision)
                {
                    mPhysicsEngine.handleCollision(Sphere(), po1->getPhysicsProperties(), po1->getTransform(), Sphere(), po2->getPhysicsProperties(), po2->getTransform());
                }
            }
        }

//        RollingBall *po2 = &mBall;

//        collision = mPhysicsEngine.CheckCollision(Sphere(), po1->getTransform(), Sphere(), po2->getTransform());

//        if(collision.collision)
//        {
//            mPhysicsEngine.handleCollision(Sphere(), po1->getPhysicsProperties(), po1->getTransform(), Sphere(), po2->getPhysicsProperties(), po2->getTransform());
//        }
    }

    RollingBall *b = &mBall;
    for(auto &enemy : mEnemies)
    {
        collision = mPhysicsEngine.CheckCollision(Sphere(), b->getTransform(), Sphere(), enemy->getTransform());

        if(collision.collision)
        {
            reset();
        }

        if(enemy->getDetectionRadius() >= collision.distance)
        {
            enemy->notify(NPC_Notification::PLAYER_DETECTED);
        }
    }

    for(auto &item : mItems)
    {
        collision = mPhysicsEngine.CheckCollision(Sphere(), b->getTransform(), Sphere(), item->getTransform());

        if(collision.collision == true)
        {
            item->Deactivate();

            bool allTaken = true;
            for(auto item : mItems)
            {
                if(item->IsActive())
                {
                    allTaken = false;
                    break;
                }
            }

            if(allTaken)
                reset();
        }
    }

    for(auto &ball : mPhysicsObjects)
    {
        collision = mPhysicsEngine.CheckCollision(Sphere(), b->getTransform(), Sphere(), ball->getTransform());

        if(collision.collision)
            mPhysicsEngine.handleCollision(Sphere(), b->getPhysicsProperties(), b->getTransform(), Sphere(), ball->getPhysicsProperties(), ball->getTransform());
    }
}

void Scene3::reset()
{
    Scene::reset();

    for(auto physic : mPhysicsObjects)
    {
        physic->setLocation(mDefaultPositions[physic]);
        physic->getPhysicsProperties().velocity = QVector3D(0.f,0.f,0.f);
    }
}

void Scene3::listObjects()
{
    //mTriangleSurface = new IndexedTriangleSurface("test_las", "none", 1.f, true);
    //mTriangleSurface->run();

    //std::vector<float> k;
    //std::vector<QVector3D> cp;
    //BSplineCurve *bc = new BSplineCurve();
    //mObjects.push_back(bc);

    std::vector<float> k1 = {0,0,1,2,3,3};
    std::vector<QVector3D> cp1 = {QVector3D(10.f, 10.f, 0.f),
                                 QVector3D(30.f, 10.f, 0.f)};
    BSplineCurve *bc0 = new BSplineCurve();
    mObjects.push_back(bc0);
    BSplineCurve *bc1 = new BSplineCurve();
    mObjects.push_back(bc1);
    //bc1->setNewValues(k1, cp1, 2);

    std::vector<Item*> is0;
    Item *i0 = new Item();
    is0.push_back(i0);
    placeObject(i0, {100.f, 600.f});
    mItems.push_back(i0);
    mObjects.push_back(i0);
    Item *i1 = new Item();
    is0.push_back(i1);
    placeObject(i1, {400.f, 600.f});
    mItems.push_back(i1);
    mObjects.push_back(i1);
    Item *i2 = new Item();
    is0.push_back(i2);
    placeObject(i2, {200.f, 400.f});
    mItems.push_back(i2);
    mObjects.push_back(i2);
    Item *i3 = new Item();
    is0.push_back(i3);
    placeObject(i3, {380.f, 300.f});
    mItems.push_back(i3);
    mObjects.push_back(i3);

    Enemy *e0 = new Enemy(bc0, &mBall);
    placeObject(e0, {210.f, 100.f});
    e0->setItemsGuarding(is0);
    mObjects.push_back(e0);
    mEnemies.push_back(e0);

    std::vector<Item*> is1;
    Item *i4 = new Item();
    is1.push_back(i4);
    placeObject(i4, {200.f, 100.f});
    mItems.push_back(i4);
    mObjects.push_back(i4);
    Item *i5 = new Item();
    is1.push_back(i5);
    placeObject(i5, {300.f, 200.f});
    mItems.push_back(i5);
    mObjects.push_back(i5);
    Item *i6 = new Item();
    is1.push_back(i6);
    placeObject(i6, {300.f, 300.f});
    mItems.push_back(i6);
    mObjects.push_back(i6);
    Item *i7 = new Item();
    is1.push_back(i7);
    placeObject(i7, {400.f, 80.f});
    mItems.push_back(i7);
    mObjects.push_back(i7);

    Enemy *e1 = new Enemy(bc1, &mBall);
    placeObject(e1, {210.f, 100.f});
    e1->setItemsGuarding(is1);
    mObjects.push_back(e1);
    mEnemies.push_back(e1);

    RollingBall *p0 = new RollingBall();
    placeObject(p0, {400.f, 930.f});
    mObjects.push_back(p0);
    mPhysicsObjects.push_back(p0);

    RollingBall *p1 = new RollingBall();
    placeObject(p1, {420.f, 910.f});
    mObjects.push_back(p1);
    mPhysicsObjects.push_back(p1);

    RollingBall *p2 = new RollingBall();
    placeObject(p2, {410.f, 860.f});
    mObjects.push_back(p2);
    mPhysicsObjects.push_back(p2);

    RollingBall *p3 = new RollingBall();
    placeObject(p3, {390.f, 860.f});
    mObjects.push_back(p3);
    mPhysicsObjects.push_back(p3);

    RollingBall *p4 = new RollingBall();
    placeObject(p4, {450.f, 830.f});
    mObjects.push_back(p4);
    mPhysicsObjects.push_back(p4);

    RollingBall *p5 = new RollingBall();
    placeObject(p5, {843.527, 995.542});
    mObjects.push_back(p5);
    mPhysicsObjects.push_back(p5);

    RollingBall *p6 = new RollingBall();
    placeObject(p6, {797.199, 875.6});
    mObjects.push_back(p6);
    mPhysicsObjects.push_back(p6);

    RollingBall *p7 = new RollingBall();
    placeObject(p7, {712.546, 1067.93});
    mObjects.push_back(p7);
    mPhysicsObjects.push_back(p7);

    RollingBall *p8 = new RollingBall();
    placeObject(p8, {832.278, 1100.24});
    mObjects.push_back(p8);
    mPhysicsObjects.push_back(p8);

    placeObject(&mBall, {400.824f, 840.358f});

    mCamera.setYaw(190.8f);
    mCamera.setPitch(-28.f);
    mCamera.setOffset(93.f);
    //mPhysicsObjects.push_back(&mBall);
}
