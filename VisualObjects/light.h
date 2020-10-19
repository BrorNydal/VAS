#ifndef LIGHT_H
#define LIGHT_H

#include "VisualObjects/visualobject.h"

struct LightUniforms{
    int mIntensityUniform;
    int mLightColorUniform;
    int mLightPositionUniform;
};

class Light : public VisualObject
{
public:
    Light();

    float mIntensity;
    Vector3D mLightColor;

    virtual void init() override;
    virtual void draw() override;

    void setIntensityUniform(GLint uniform) {mLightUniforms.mIntensityUniform = uniform;}
    void setLightColorUniform(GLint uniform) {mLightUniforms.mLightColorUniform = uniform;}
    void setLightPositionUniform(GLint uniform) {mLightUniforms.mLightPositionUniform = uniform;}

protected:
    LightUniforms mLightUniforms;
};

#endif // LIGHT_H
