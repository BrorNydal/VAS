#include "contourline.h"
#include "trianglesurface.h"
#include "indexedtrianglesurface.h"
#include <algorithm>

ContourLine::ContourLine(VisualObject *triangleSurface, float height, bool indexed)
    :   mSurface(triangleSurface), mHeight(height), mIndexedTerrain(indexed)
{

}

void ContourLine::init()
{
    mMatrix.setToIdentity();

    initializeOpenGLFunctions();

    initDrawLines();

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

void ContourLine::draw()
{
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.data());
    glBindVertexArray( mVAO );
    glDrawElements(GL_LINES, static_cast<int>(mIndices.size()), GL_UNSIGNED_INT, NULL);
}

void ContourLine::createContourLine()
{
    //If both TriangleSurface and IndexedTriangleSurface inherited from one class, we wouldnt need to create these branches
    if(mIndexedTerrain)
    {
        IndexedTriangleSurface *surface = static_cast<IndexedTriangleSurface*>(mSurface);

        for(unsigned int triangleIndex = 0; triangleIndex < surface->getTriangles().size(); triangleIndex++)
        {
            Triangle triangle = surface->getTriangles()[triangleIndex];

            Vertex v0 = surface->getVertex(triangle.mIndecies[0]);
            Vertex v1 = surface->getVertex(triangle.mIndecies[1]);
            Vertex v2 = surface->getVertex(triangle.mIndecies[2]);

            Edge e0 = Edge(Vector3D(v0.x, v0.y, v0.z), Vector3D(v1.x, v1.y, v1.z));
            Edge e1 = Edge(Vector3D(v1.x, v1.y, v1.z), Vector3D(v2.x, v2.y, v2.z));
            Edge e2 = Edge(Vector3D(v2.x, v2.y, v2.z), Vector3D(v0.x, v0.y, v0.z));

            if(std::find(mEdges.begin(), mEdges.end(), e0) == mEdges.end())
            {
                if((e0.mFrom.z() <= mHeight && e0.mTo.z() >= mHeight) || (e0.mTo.z() <= mHeight && e0.mFrom.z() >= mHeight))
                {
                    mEdges.push_back(e0);
                }
            }
            if(std::find(mEdges.begin(), mEdges.end(), e1) == mEdges.end())
            {
                if((e1.mFrom.z() <= mHeight && e1.mTo.z() >= mHeight) || (e1.mTo.z() <= mHeight && e1.mFrom.z() >= mHeight))
                {
                    mEdges.push_back(e1);
                }
            }
            if(std::find(mEdges.begin(), mEdges.end(), e2) == mEdges.end())
            {
                if((e2.mFrom.z() <= mHeight && e2.mTo.z() >= mHeight) || (e2.mTo.z() <= mHeight && e2.mFrom.z() >= mHeight))
                {
                    mEdges.push_back(e2);
                }
            }
        }
    }
    else
    {
        TriangleSurface *surface = static_cast<TriangleSurface*>(mSurface);

        for(unsigned int triangleIndex = 0; triangleIndex < surface->getTriangles().size(); triangleIndex++)
        {
            OldTriangle triangle = surface->getTriangles()[triangleIndex];

            Edge e0 = Edge(triangle.mPositions[0], triangle.mPositions[1]);
            Edge e1 = Edge(triangle.mPositions[1], triangle.mPositions[2]);
            Edge e2 = Edge(triangle.mPositions[2], triangle.mPositions[0]);

            if(std::find(mEdges.begin(), mEdges.end(), e0) == mEdges.end())
            {
                if((e0.mFrom.z() <= mHeight && e0.mTo.z() >= mHeight) || (e0.mTo.z() <= mHeight && e0.mFrom.z() >= mHeight))
                {
                    mEdges.push_back(e0);
                }
            }
            if(std::find(mEdges.begin(), mEdges.end(), e1) == mEdges.end())
            {
                if((e1.mFrom.z() <= mHeight && e1.mTo.z() >= mHeight) || (e1.mTo.z() <= mHeight && e1.mFrom.z() >= mHeight))
                {
                    mEdges.push_back(e1);
                }
            }
            if(std::find(mEdges.begin(), mEdges.end(), e2) == mEdges.end())
            {
                if((e2.mFrom.z() <= mHeight && e2.mTo.z() >= mHeight) || (e2.mTo.z() <= mHeight && e2.mFrom.z() >= mHeight))
                {
                    mEdges.push_back(e2);
                }
            }
        }
    }

    for(unsigned int i = 0; i < mEdges.size(); i++)
    {
        if(mEdges[i].mBetween.z() < 0)
        {
            mEdges[i].mBetween = mEdges[i].mBetween * -1;
            Vector3D temp = mEdges[i].mTo;
            mEdges[i].mTo = mEdges[i].mFrom;
            mEdges[i].mFrom = temp;
        }

        //mHeight - 1 cause we calculate from z = 0 and the lowest vertex is 1
        //If groundlevel = 0 we could just use mHeigth
        float delta = (mHeight - 1) / mEdges[i].mBetween.z();
        Vector3D result = mEdges[i].mTo * delta + mEdges[i].mFrom * (1 - delta);
        qDebug() << "Contour result:" << result.x() << "," << result.y() << "," << result.z();
        mVertices.push_back(Vertex(result.x(), result.y(), result.z() + 0.01f));
    }
}

void ContourLine::initDrawLines()
{
//    for(unsigned int i = 0; i < mVertices.size() - 1; i++)
//    {
//        mIndices.push_back(i);
//        mIndices.push_back(i + 1);
//    }

    mIndices.push_back(0);
    mIndices.push_back(1);
    mIndices.push_back(1);
    mIndices.push_back(2);
    mIndices.push_back(2);
    mIndices.push_back(6);
    mIndices.push_back(6);
    mIndices.push_back(0);

    mIndices.push_back(3);
    mIndices.push_back(4);
    mIndices.push_back(4);
    mIndices.push_back(5);
    mIndices.push_back(5);
    mIndices.push_back(11);
    mIndices.push_back(11);
    mIndices.push_back(3);

    mIndices.push_back(7);
    mIndices.push_back(8);
    mIndices.push_back(8);
    mIndices.push_back(10);
    mIndices.push_back(10);
    mIndices.push_back(9);
    mIndices.push_back(9);
    mIndices.push_back(7);
}
