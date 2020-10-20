#ifndef LINEDRAW_H
#define LINEDRAW_H

#include "VisualObjects/visualobject.h"

class LineDraw : public VisualObject
{
public:
    LineDraw();

    virtual void init() override;
    virtual void draw() override;

    void addSetOfPositions(std::vector<Vector3D> positions);
    void addPosition(Vector3D pos);
    void lerpBetweenPositions(VisualObject *object, float speed = 0.01f);

protected:

    float mLerpDelta;
    unsigned int mLerpIndex;
};

#endif // LINEDRAW_H
