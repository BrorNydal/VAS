#include "linedraw.h"

LineDraw::LineDraw()
    :   mLerpDelta(0.f), mLerpIndex(0)
{

}

void LineDraw::init()
{
    mMatrix.setToIdentity();

    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mEAB);
    glGenBuffers(1, &mVBO);

    //what object to draw
    glBindVertexArray(mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(mVertices.size()*sizeof( Vertex )), mVertices.data(), GL_STATIC_DRAW );

    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<unsigned int>(mIndices.size()) * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);
}

void LineDraw::draw()
{
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.data());
    glBindVertexArray( mVAO );
    glDrawElements(GL_LINES, static_cast<int>(mIndices.size()), GL_UNSIGNED_INT, NULL);
}

void LineDraw::addSetOfPositions(std::vector<Vector3D> positions)
{
    for(unsigned int i = 0; i < positions.size(); i++)
    {
        mVertices.push_back(Vertex(positions[i].x(), positions[i].y(), positions[i].z()));
        mIndices.push_back(i);
    }
}

void LineDraw::addPosition(Vector3D pos)
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
        Vector3D result;

        unsigned int indexTo    = mLerpIndex + 1;
        unsigned int indexFrom  = mLerpIndex;

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
            }
        }
    }
    else
    {
        qDebug() << "[WANRING] Trying to lerp out of range in lerpBetweenPositions(VisualObject*, float)";
    }
}
