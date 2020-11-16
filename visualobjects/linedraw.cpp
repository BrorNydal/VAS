#include "linedraw.h"

LineDraw::LineDraw()
    :   mLerpDelta(0.f), mLerpIndex(0)
{
    mDrawMode = GL_LINES;
}


void LineDraw::addSetOfPositions(std::vector<QVector3D> positions)
{
    for(unsigned int i = 0; i < positions.size(); i++)
    {
        mVertices.push_back(Vertex(positions[i].x(), positions[i].y(), positions[i].z()));
        mIndices.push_back(i);
    }
}

void LineDraw::addPosition(QVector3D pos)
{
    mVertices.push_back(Vertex(pos.x(), pos.y(), pos.z()));

    if(mVertices.size() > 0)
    {
        mIndices.push_back(static_cast<unsigned int>(mVertices.size() - 1));
        mIndices.push_back(static_cast<unsigned int>(mVertices.size()));
    }
}

void LineDraw::lerpBetweenPositions(VisualObject *object, float speed)
{
    if(mLerpIndex + 1 < mVertices.size())
    {
        QVector3D result;

        unsigned int indexTo    = mLerpIndex + 1;
        unsigned int indexFrom  = mLerpIndex;

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
            }
        }
    }
    else
    {
        qDebug() << "[WANRING] Trying to lerp out of range in lerpBetweenPositions(VisualObject*, float)";
    }
}
