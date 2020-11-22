#include "scene.h"

Scene::Scene()
{
    initializeOpenGLFunctions();

    mShaders[EShader::plain] = new Shader("../VAS/plainvertex.vert", "../VAS/plainfragment.frag", EShader::plain);
    mShaders[EShader::phong] = new Shader("../VAS/PhongVertex.vert", "../VAS/PhongFragment.frag", EShader::phong);
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
    mBall.setLocation({4.f, 1.f, 0.f});
    mBall.getPhysicsProperties().velocity = {0.f,0.f,0.f};

}

void Scene::initializeObjects()
{
    mXYZ.init();
    mGrid.init();
    mBall.init();
    mLight.init();

    if(mTriangleSurface != nullptr)
        mObjects.push_back(mTriangleSurface);

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
        glUseProgram(mShaders[(*it)->getShader()]->getID());
        current = (*it)->getShader();

        mCamera.render(*mShaders[current]);

        (*it)->draw(*mShaders[(*it)->getShader()]);
    }

    if(mLockCameraToBall)
        mCamera.setLocation(mBall.getLocation());

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
