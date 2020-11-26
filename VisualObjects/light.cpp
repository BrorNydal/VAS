#include "light.h"

Light::Light()
{
    mDrawArrays = true;

    mVertices.push_back(Vertex(0.f, 0.f, 0.f, 0.f, 0.f, -1.f));
    mVertices.push_back(Vertex(1.f, 0.f, 0.f, 0.f, 0.f, -1.f));
    mVertices.push_back(Vertex(0.5f, 0.8f, 0.f, 0.f, 0.f, -1.f));
}

void Light::draw(Shader &shader)
{
    shader.uniform3f("light.direction", mLightProperties.direction);
    shader.uniform3f("light.ambient",   mLightProperties.ambient);
    shader.uniform3f("light.diffuse",   mLightProperties.diffuse);
    shader.uniform3f("light.specular",  mLightProperties.specular);
}

const LightProperties &Light::getLightProperties() const
{
    return mLightProperties;
}
