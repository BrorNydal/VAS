#include "Scenes/scene2.h"

#include "VisualObjects/xyz.h"
#include "VisualObjects/grid.h"
#include "VisualObjects/beziercurve.h"
#include "VisualObjects/octahedronball.h"
#include "VisualObjects/light.h"

Scene2::Scene2()
{
    initializeOpenGLFunctions();

    //Camera Position
    mViewMatrix(0, 3) = 0.f;
    mViewMatrix(1, 3) = 0.f;
    mViewMatrix(2, 3) = -8.f;
}

void Scene2::draw()
{
    Scene::draw();
}

void Scene2::setUniforms()
{
    Scene::setUniforms();

}

void Scene2::listObjects()
{
    Scene::listObjects();
}

void Scene2::setTransformations()
{
    Scene::setTransformations();
}

