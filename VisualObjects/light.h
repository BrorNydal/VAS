#ifndef LIGHT_H
#define LIGHT_H

#include "VisualObjects/visualobject.h"
#include <QVector3D>


class Light : public VisualObject
{
    LightProperties mLightProperties;

public:
    Light();

    virtual void draw(Shader &shader) override;

    const LightProperties &getLightProperties() const;

};

#endif // LIGHT_H
