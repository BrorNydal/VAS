#include "scene.h"

Scene::Scene(IndexedTriangleSurface *ts)
{
    initializeOpenGLFunctions();

    mShaders[EShader::plain] = new Shader("../VAS/plainvertex.vert", "../VAS/plainfragment.frag", EShader::plain);
    mShaders[EShader::phong] = new Shader("../VAS/PhongVertex.vert", "../VAS/PhongFragment.frag", EShader::phong);

    if(ts != nullptr)
        setSurface(ts);
}

Scene::~Scene()
{
    for(auto *it : mObjects)
        delete it;
}

void Scene::initializeScene()
{
    listObjects();
    initializeObjects();

    mInitialized = true;
}

void Scene::reset()
{
    placeObject(&mBall, mDefaultPositions[&mBall]);
    mBall.getPhysicsProperties().velocity = {0.f,0.f,0.f};

    for(auto &obj : mObjects)
    {
        if(!dynamic_cast<IndexedTriangleSurface*>(obj))
        {
            if(dynamic_cast<RollingBall*>(obj))
                dynamic_cast<RollingBall*>(obj)->getPhysicsProperties().velocity = QVector3D(0.f,0.f,0.f);

            obj->Activate();

            placeObject(obj, mDefaultPositions[obj]);
        }
    }
}

void Scene::initializeObjects()
{
    mXYZ.init();
    mGrid.init();
    mBall.init();
    mLight.init();

//    if(mTriangleSurface != nullptr)
//        mObjects.push_back(mTriangleSurface);

    for(unsigned int i = 0; i < mObjects.size(); i++)
        mObjects[i]->init();
}

void Scene::draw(float deltaTime)
{
    EShader current = EShader::plain;
    mShaders[current]->use();

    mCamera.lookAt();
    mCamera.render(*mShaders[current]);

    mXYZ.draw(*mShaders[current]);
    mGrid.draw(*mShaders[current]);

    current = EShader::phong;
    mShaders[current]->use();
    mCamera.render(*mShaders[current]);

    mLight.draw(*mShaders[current]);
    mBall.draw(*mShaders[current]);

    for(auto it = mObjects.begin(); it != mObjects.end(); it++)
    {
        if((*it) != nullptr)
        {
            if((*it)->IsActive())
            {
                glUseProgram(mShaders[(*it)->getShader()]->getID());
                current = (*it)->getShader();

                mCamera.render(*mShaders[current]);

                (*it)->draw(*mShaders[(*it)->getShader()]);
            }
        }
        else
            qDebug() << "Object nullptr";
    }

    if(mLockCameraToBall)
        mCamera.setLocation(mBall.getLocation());

}

void Scene::placeObject(VisualObject *obj, QVector2D loc)
{
    if(obj != nullptr && mTriangleSurface != nullptr)
    {
        obj->setLocation({loc.x(), loc.y(), 0.f});

        if(dynamic_cast<RollingBall*>(obj))
            obj->getTransform().location.setZ(mTriangleSurface->heightAtLocation(loc.x(), loc.y()) + 1.f);
        else
            obj->getTransform().location.setZ(mTriangleSurface->heightAtLocation(loc.x(), loc.y()));

        mDefaultPositions[obj] = loc;
    }
}

void Scene::togglePause()
{
    if(mPause)
        mPause = false;
    else
        mPause = true;
}

bool Scene::isPaused() const
{
    return mPause;
}

void Scene::setSurface(IndexedTriangleSurface *surface)
{
    if(surface != nullptr)
    {
        mTriangleSurface = surface;
        mObjects.push_back(mTriangleSurface);
        mTriangleSurface->init();
    }
}

Shader *Scene::getShader(EShader type)
{
    return mShaders[type];
}

Camera &Scene::getCamera()
{
    return mCamera;
}

void Scene::toggleCameraLookAtBall()
{
    if(mLockCameraToBall == true)
        mLockCameraToBall = false;
    else
        mLockCameraToBall = true;
}

RollingBall &Scene::getBall()
{
    return mBall;
}
