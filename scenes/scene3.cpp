#include "scene3.h"

#include "obj.h"
#include "structs.h"

Scene3::Scene3()
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
            mPhysicsEngine.handleCollision(Sphere(), b->getPhysicsProperties(), b->getTransform(), Sphere(), enemy->getPhysicsProperties(), enemy->getTransform());
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
            item->Deactivate();
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
    mTriangleSurface = new IndexedTriangleSurface("test_las", "none", 1.f, true);
    mTriangleSurface->run();

    //std::vector<float> k;
    //std::vector<QVector3D> cp;
    //BSplineCurve *bc = new BSplineCurve();
    //mObjects.push_back(bc);

    std::vector<float> k1 = {0,0,1,2,3,3};
    std::vector<QVector3D> cp1 = {QVector3D(10.f, 10.f, 0.f),
                                 QVector3D(30.f, 10.f, 0.f)};
    BSplineCurve *bc1 = new BSplineCurve();
    mObjects.push_back(bc1);
    //bc1->setNewValues(k1, cp1, 2);

    Item *i0 = new Item();
    i0->setLocation({5.f, 5.f, 0.f});
    mItems.push_back(i0);
    mObjects.push_back(i0);
    Item *i1 = new Item();
    i1->setLocation({30.f, 5.f, 0.f});
    mItems.push_back(i1);
    mObjects.push_back(i1);
    Item *i2 = new Item();
    i2->setLocation({50.f, 30.f, 0.f});
    mItems.push_back(i2);
    mObjects.push_back(i2);
    Item *i3 = new Item();
    i3->setLocation({5.f, 30.f, 0.f});
    mItems.push_back(i3);
    mObjects.push_back(i3);

    Enemy *e0 = new Enemy(bc1, &mBall);
    e0->setItemsGuarding(mItems);
    mObjects.push_back(e0);
    mEnemies.push_back(e0);
    e0->setLocation({10.f, 20.f, 0.f});
    //e0->findPath();

    RollingBall *p0 = new RollingBall();
    mDefaultPositions[p0] = QVector3D(50.f, 50.f, 0.f);
    mObjects.push_back(p0);
    mPhysicsObjects.push_back(p0);
    p0->setLocation(mDefaultPositions[p0]);

    mBall.setLocation({20.f,80.f,0.f});
    //mPhysicsObjects.push_back(&mBall);
}
