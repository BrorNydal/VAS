#include "Scenes/Scene.h"

#include "GlobalConstants.h"

#include "VisualObjects/xyz.h"
#include "VisualObjects/grid.h"
#include "VisualObjects/light.h"

Scene::Scene()
    :   mXYZ(new XYZ()), mGrid(new Grid(9, 9)), mLight(new Light()), mEditorMode(false)
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

    mPlainShader = new Shader((GlobalConstans::ProjectDirectory + "plainvertex.vert").c_str(),
                              (GlobalConstans::ProjectDirectory + "plainfragment.frag").c_str());
    mPhongShader = new Shader((GlobalConstans::ProjectDirectory + "PhongVertex.vert").c_str(),
                              (GlobalConstans::ProjectDirectory + "PhongFragment.frag").c_str());

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

    for(auto object = mObjects.begin(); object != mObjects.end(); object++)
        (*object)->init();
}

void Scene::setTransformations()
{

}

void Scene::draw()
{    
    for(auto object = mObjects.begin(); object != mObjects.end(); object++)
    {
        unsigned int index = (*object)->getShaderIndex();
        glUseProgram(mShaders[index]->getProgram());

        glUniformMatrix4fv(mPlainViewUniform, 1, GL_FALSE, mViewMatrix.data());
        glUniformMatrix4fv(mPlainProjectionUniform, 1, GL_FALSE, mProjectionMatrix.data());

        glUniformMatrix4fv(mPhongViewUniform, 1, GL_FALSE, mViewMatrix.data());
        glUniformMatrix4fv(mPhongProjectionUniform, 1, GL_FALSE, mProjectionMatrix.data());

        (*object)->draw();

        if(mShaders[index] == mPhongShader)
        {
            glUniform3f(mViewPosition, -mViewMatrix(0, 3), -mViewMatrix(1, 3), -mViewMatrix(2, 3));
        }
    }
}

void Scene::listObjects()
{
    mObjects.push_back(mXYZ);
    mObjects.push_back(mGrid);
    mObjects.push_back(mLight);

    mEditorObjects.push_back(mXYZ);
    mEditorObjects.push_back(mGrid);

    mPlayObjects.push_back(mLight);
}

void Scene::setUniforms()
{
    mXYZ->setTransformMatrixUniform(mPlainTransformMatrixUniform);
    mGrid->setTransformMatrixUniform(mPlainTransformMatrixUniform);

    //LIGHT
    mLight->setTransformMatrixUniform(mPhongTransformMatrixUniform);
    mLight->setIntensityUniform(mLightIntensityUniform);
    mLight->setLightColorUniform(mLightColorUniform);
    mLight->setLightPositionUniform(mLightPositionUniform);
}

void Scene::createAndInitialize()
{
    listObjects();

    init();

    setTransformations();
}
