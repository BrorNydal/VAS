#include "beziercurve.h"

BezierCurve::BezierCurve()
    :   mDisplayBezierCurve(false), mControlPointsColor(1.f, 0.f, 0.f),
        mToControlPoint(5), mStart(0.f), mEnd(1.f), mLength(mEnd - mStart),
        mIterations(400), mBezierCurveColor(0.f, 1.f, 0.f), mLerpIndex(0), mLerpDelta(0.f)
{
    createBezierCurve();
}

void BezierCurve::initializeBezierPositions()
{
    mControlPoints.push_back(QVector3D(0, 0, 4));
    mControlPoints.push_back(QVector3D(-3.5, 2.5, 4));
    mControlPoints.push_back(QVector3D(0, 5, 4));
    mControlPoints.push_back(QVector3D(5, 5, 4));
    mControlPoints.push_back(QVector3D(5, 0, 4));
    mControlPoints.push_back(QVector3D(0, 0, 4));

    mControlPointsSize = static_cast<unsigned long>(mControlPoints.size());
}

QVector3D BezierCurve::evaluateBezier(unsigned int degree, float t)
{
    std::vector<QVector3D> c;

    if(mControlPoints.size() == 0)
    {
        //qDebug() << "ControlPoint size null";
        return QVector3D();
    }

    for(unsigned int i = 0; i < mControlPoints.size(); i++)
        c.push_back(mControlPoints[i]);

    for(unsigned int k = 1; k <= degree; k++)
    {
        for(unsigned int i = 0; i < degree - k+1; i++)
            c[i] = c[i] * (1 - t) + c[i +1]*t;
    }

    return c[0];
}

QVector3D BezierCurve::getLocationOnBezierCurve(unsigned int i)
{
    if(i + mControlPointsSize < mVertices.size())
    {
        QVector3D v = mTransform.location * QVector3D(mVertices[i + mControlPointsSize].x,
                mVertices[i + mControlPointsSize].y,
                mVertices[i + mControlPointsSize].z);

        return QVector3D(v.x(), v.y(), v.z());
    }
    else
        return QVector3D(0.f,0.f,0.f);
}

QVector3D BezierCurve::getControlPointPosition(unsigned int index)
{
    if(index < mControlPointsSize)
    {
        return QVector3D(mVertices[index].x, mVertices[index].y, mVertices[index].z);
    }

    return QVector3D(mVertices[0].x, mVertices[0].y, mVertices[0].z);
}

bool BezierCurve::lerpBetweenPositions(VisualObject *object, float speed)
{
    if(mControlPointsSize + mLerpIndex + 1 < mVertices.size())
    {
        QVector3D result;

        unsigned int indexTo    = mControlPointsSize + mLerpIndex + 1;
        unsigned int indexFrom  = mControlPointsSize + mLerpIndex;

        QVector3D to     = QVector3D(mVertices[indexTo].x, mVertices[indexTo].y, mVertices[indexTo].z);
        QVector3D from   = QVector3D(mVertices[indexFrom].x, mVertices[indexFrom].y, mVertices[indexFrom].z);

        result = to * mLerpDelta + from * (1 - mLerpDelta);

        object->setLocation(result);
        mLerpDelta += speed / (to - from).length(); //Dividing by length so the objects moves at constant speed

        if(mLerpDelta >= 1)
        {
            mLerpDelta = 0;

            mLerpIndex++;

            if(indexTo == mVertices.size() - 1)
            {
                mLerpIndex   = 0;
                return true;
            }
        }
    }
    else
    {
        qDebug() << "[WANRING] Trying to lerp out of range in lerpBetweenPositions(VisualObject*, float)";
    }

    return false;
}

void BezierCurve::setControlPoints(std::vector<QVector3D> newControlPoints)
{
    mControlPoints.clear();

    for(unsigned int i = 0; i < newControlPoints.size(); i++)
    {
        mControlPoints.push_back(newControlPoints[i]);
    }

    mControlPointsSize = newControlPoints.size();
    mToControlPoint = mControlPointsSize - 1;

    initializeIndices();
}

void BezierCurve::createBezierCurve()
{
    float t = mStart;
    mAccuracy = mLength / mIterations;
    unsigned int i = mControlPointsSize;

    while(t <= mEnd) {
        QVector3D v = evaluateBezier(mToControlPoint, t);

        mVertices.push_back(Vertex(v.x(), v.y(), v.z()));

        //Indices
        mIndices.push_back(i);

        if(i < mIterations + mControlPointsSize - 1)
            mIndices.push_back(i + 1);

        i++;
        t += mAccuracy;
    }
}

void BezierCurve::initializeIndices()
{
    for(unsigned int i = 0; i < mControlPointsSize; i++)
    {
        mVertices.push_back(Vertex(mControlPoints[i].x(), mControlPoints[i].y(), mControlPoints[i].z()));

        //Indices
        if(i < mControlPointsSize - 1)
        {
            mIndices.push_back(static_cast<unsigned int>(i));
            mIndices.push_back(static_cast<unsigned int>(i + 1));
        }
    }
}
