#include "Scenes/Scene.h"

Scene::Scene()
{
    initializeScene();
}

Scene::~Scene()
{
    std::vector<VisualObject*> empty;
    mObjects.swap(empty);
}

void Scene::initializeScene()
{
    initializeOpenGLFunctions();

    mPlainShader = new Shader("../GSOpenGL2020/plainvertex.vert", "../GSOpenGL2020/plainfragment.frag");
    mPhongShader = new Shader("../GSOpenGL2020/PhongVertex.vert", "../GSOpenGL2020/PhongFragment.frag");
    mShaders.push_back(mPlainShader);
    mShaders.push_back(mPhongShader);

    //TRANSFORMATION MATRIX

    mPlainTransformMatrixUniform = glGetUniformLocation( mPlainShader->getProgram(), "matrix"   );
    mPhongTransformMatrixUniform = glGetUniformLocation( mPhongShader->getProgram(), "matrix"   );

    //CAMERA

    mPlainViewUniform       = glGetUniformLocation( mPlainShader->getProgram(), "vmatrix"       );
    mPlainProjectionUniform = glGetUniformLocation( mPlainShader->getProgram(), "pmatrix"       );

    mPhongViewUniform       = glGetUniformLocation( mPhongShader->getProgram(), "vmatrix"       );
    mPhongProjectionUniform = glGetUniformLocation( mPhongShader->getProgram(), "pmatrix"       );
    mViewPosition           = glGetUniformLocation( mPhongShader->getProgram(), "viewPosition"  );

    mProjectionMatrix.frustum(-1, 1,-1, 1, 1, -1);

    //LIGHT

    mLightPositionUniform   = glGetUniformLocation( mPhongShader->getProgram(), "lightPosition" );
    mLightIntensityUniform  = glGetUniformLocation( mPhongShader->getProgram(), "lightIntensity");
    mLightColorUniform      = glGetUniformLocation( mPhongShader->getProgram(), "lightColor"    );
}

void Scene::init()
{
    setUniforms();

    for(unsigned int i = 0; i < mObjects.size(); i++)
        mObjects[i]->init();
}

void Scene::draw()
{
    for(auto it = mObjects.begin(); it != mObjects.end(); it++)
    {
        unsigned int index = (*it)->getShaderIndex();
        glUseProgram(mShaders[index]->getProgram());

        glUniformMatrix4fv(mPlainViewUniform, 1, GL_FALSE, mViewMatrix.data());
        glUniformMatrix4fv(mPlainProjectionUniform, 1, GL_FALSE, mProjectionMatrix.data());

        glUniformMatrix4fv(mPhongViewUniform, 1, GL_FALSE, mViewMatrix.data());
        glUniformMatrix4fv(mPhongProjectionUniform, 1, GL_FALSE, mProjectionMatrix.data());

        (*it)->draw();

        if(mShaders[index] == mPhongShader)
        {
            glUniform3f(mViewPosition, 0.f, 0.f, 8.f);
        }
    }

    if(mTurnTable){
        for(auto it = mObjects.begin(); it != mObjects.end(); it++)
        {
            (*it)->rotateZ(0.2f);
        }
    }
}

void Scene::createAndInitialize()
{
    listObjects();

    init();

    setTransformations();

    mInitialized = true;
}
