#ifndef LINEDRAW_H
#define LINEDRAW_H

#include "VisualObjects/visualobject.h"

class LineDraw : public VisualObject
{
public:
    LineDraw();

    void addSetOfPositions(std::vector<QVector3D> positions);
    void addPosition(QVector3D pos);
    void lerpBetweenPositions(VisualObject *object, float speed = 0.01f);

protected:

    float mLerpDelta;
    unsigned int mLerpIndex;
};

#endif // LINEDRAW_H
