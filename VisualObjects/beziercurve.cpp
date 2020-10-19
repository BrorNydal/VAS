#include "beziercurve.h"

BezierCurve::BezierCurve()
    :   mDisplayBezierCurve(false), mControlPointsColor(1.f, 0.f, 0.f),
        mToControlPoint(5), mStart(0.f), mEnd(1.f), mLength(mEnd - mStart),
        mIterations(400), mBezierCurveColor(0.f, 1.f, 0.f), mLerpIndex(0), mLerpDelta(0.f)
{
}

void BezierCurve::init()
{
    mMatrix.setToIdentity();

    initializeOpenGLFunctions();

    createBezierCurve();

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mEAB);
    glGenBuffers(1, &mVBO);

    //what object to draw
    glBindVertexArray(mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<unsigned int>(mIndices.size()) * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);
}

void BezierCurve::draw()
{
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.data());
    glBindVertexArray( mVAO );

    if(mDisplayBezierCurve)
        glDrawElements(GL_LINES, mIndices.size(), GL_UNSIGNED_INT, NULL);
}

void BezierCurve::initializeBezierPositions()
{
    mControlPoints.push_back(Vector3D(0,    0,  4));
    mControlPoints.push_back(Vector3D(-3.5,    2.5,  4));
    mControlPoints.push_back(Vector3D(0,    5, 4));
    mControlPoints.push_back(Vector3D(5,   5, 4));
    mControlPoints.push_back(Vector3D(5,   0,  4));
    mControlPoints.push_back(Vector3D(0,    0,  4));

    mControlPointsSize = static_cast<unsigned long>(mControlPoints.size());
}

Vector3D BezierCurve::evaluateBezier(unsigned int degree, float t)
{
    std::vector<Vector3D> c;
    for(unsigned int i = 0; i < mControlPointsSize; i++)
        c.push_back(mControlPoints[i]);

    for(unsigned int k = 1; k <= degree; k++)
    {
        for(unsigned int i = 0; i < degree - k+1; i++)
            c[i] = c[i] * (1 - t) + c[i +1]*t;
    }

    return c[0];
}

Vector3D BezierCurve::getLocationOnBezierCurve(unsigned int i)
{
    if(i + mControlPointsSize < mVertices.size())
    {
        QVector3D v = mMatrix * QVector3D(mVertices[i + mControlPointsSize].x,
                mVertices[i + mControlPointsSize].y,
                mVertices[i + mControlPointsSize].z);

        return Vector3D(v.x(), v.y(), v.z());
    }
    else
        return Vector3D(0.f,0.f,0.f);
}

Vector3D BezierCurve::getControlPointPosition(unsigned int index)
{
    if(index < mControlPointsSize)
    {
        return Vector3D(mVertices[index].x, mVertices[index].y, mVertices[index].z);
    }

    return Vector3D(mVertices[0].x, mVertices[0].y, mVertices[0].z);
}

bool BezierCurve::lerpBetweenPositions(VisualObject *object, float speed)
{
    if(mControlPointsSize + mLerpIndex + 1 < mVertices.size())
    {
        Vector3D result;

        unsigned int indexTo    = mControlPointsSize + mLerpIndex + 1;
        unsigned int indexFrom  = mControlPointsSize + mLerpIndex;

        Vector3D to     = Vector3D(mVertices[indexTo].x, mVertices[indexTo].y, mVertices[indexTo].z);
        Vector3D from   = Vector3D(mVertices[indexFrom].x, mVertices[indexFrom].y, mVertices[indexFrom].z);

        result = to * mLerpDelta + from * (1 - mLerpDelta);

        object->setPosition(result);
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

void BezierCurve::setControlPoints(std::vector<Vector3D> newControlPoints)
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
        Vector3D v = evaluateBezier(mToControlPoint, t);

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
