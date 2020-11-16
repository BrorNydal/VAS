#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "VisualObjects/visualobject.h"

class BezierCurve : public VisualObject
{
public:
    BezierCurve();

    QVector3D evaluateBezier(unsigned int degree, float t);

    QVector3D getLocationOnBezierCurve(unsigned int i);

    unsigned int getBezierCurveVectorSize() {return static_cast<unsigned long>(mVertices.size() - mControlPointsSize);}

    QVector3D getControlPointPosition(unsigned int index);

    bool lerpBetweenPositions(VisualObject *object, float speed = 0.01f); //return delta

    void setControlPoints(std::vector<QVector3D> newControlPoints);
    void setIterations(unsigned int it) {mIterations = it;}
    void setStartAndEnd(float start, float end) {mStart = start; mEnd = end;}
    void setDisplayBezierCurve(bool value) {mDisplayBezierCurve = value;}


protected:
    bool mDisplayBezierCurve;

    QVector3D mControlPointsColor;
    std::vector<QVector3D> mControlPoints;
    //Destination for the curve
    unsigned int mToControlPoint;

    //Everything between 0 and 1 is the desired curve
    float mStart, mEnd;
    float mLength;

    //How smooth the curve is
    unsigned int mIterations;

    QVector3D mBezierCurveColor;

    unsigned int mLerpIndex;
    float mLerpDelta;

    void createBezierCurve();
    void initializeIndices();
    void initializeBezierPositions();
    unsigned int mControlPointsSize;
    float mAccuracy;
};

#endif // BEZIERCURVE_H
