#include "terrain.h"
#include <cmath>
#include "math.h"
#include <gl/GL.h>

Terrain::Terrain()
    :   mCol(10), mRow(10), mTerrainSize(mCol*mRow)
{
    initializeOpenGLFunctions();
}

Terrain::Terrain(unsigned int col, unsigned int row, float squareSize)
    :   mCol(col), mRow(row), mSquareSize(squareSize), mTerrainSize(mCol*mRow)
{
    initializeOpenGLFunctions();

}

void Terrain::init()
{
    mMatrix.setToIdentity();

    mName = std::string("Terrain");
    mShaderIndex = 1;

    createTerrain();
    makeTextureRepeat();
    initializeDrawGLTriangles();

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mEAB);
    glGenBuffers(1, &mVBO);

    //what object to draw
    glBindVertexArray(mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(mVertices.size()*sizeof( Vertex )), mVertices.data(), GL_STATIC_DRAW );

    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof( Vertex ), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ),  (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<unsigned int>(mIndices.size()) * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);
}

void Terrain::draw()
{
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.data());
    glBindVertexArray( mVAO );
    glDrawElements(GL_TRIANGLES, static_cast<int>(mIndices.size()), GL_UNSIGNED_INT, NULL);
}

void Terrain::initializeDrawGLLines()
{
    //NEED OPTIMALIZATION

    mDrawMode = GL_LINES;

    mIndices.clear();

    int i = 0;
    for(unsigned int y = 0; y < mRow; y++)
    {
        for(unsigned int x = 0; x < mCol; x++)
        {
            if(x != mCol - 1)
            {
                mIndices.push_back(i);
                mIndices.push_back(i + 1);
            }
            i++;
        }
    }

    for(unsigned int y = 0; y < mCol; y++)
    {
        for(unsigned int x = 0; x < mRow; x++)
        {
            if(x != mRow - 1)
            {
                mIndices.push_back(y + (x * mCol));
                mIndices.push_back(y + ((x + 1) * mCol));
            }
        }
    }

    for(unsigned int y = 0; y < mRow; y++)
    {
        for(unsigned int x = 0; x < mCol; x++)
        {
            if(x != mCol - 1 && y != mRow - 1)
            {
                mIndices.push_back(x + (y * mCol));
                mIndices.push_back((x + 1) + ((y + 1) * mCol));
            }
        }
    }
}

void Terrain::initializeDrawGLTriangles()
{
    mIndices.clear();

    for(unsigned int y = 0; y < mRow - 1; y++)
    {
        for(unsigned int x = 0; x < mCol - 1; x++)
        {
                mIndices.push_back( (y * mCol) + x                      );
                mIndices.push_back( (y * mCol) + x + (mCol)             );
                mIndices.push_back( (y * mCol) + (x + 1)                );

                mIndices.push_back( (y * mCol) + (x + 1)                );
                mIndices.push_back( (y * mCol) + (x + 1) + (mCol)       );
                mIndices.push_back( (y * mCol) + (x + 1) + (mCol - 1)   );
        }
    }
}

float Terrain::getHeightAtPosition(float x, float y)
{
    //Where we are relative to the terrain
    float resultX = x - getPosition().x();
    float resultY = y - getPosition().y();

    //Which square we are staning on
    int gridX = (int) floorf(resultX / mSquareSize);
    int gridY = (int) floorf(resultY / mSquareSize);

    if(gridX >= mCol - 1 || gridY >= mRow - 1 || gridX < 0 || gridY < 0)
    {
        return 0.f;
    }

    float squareCoordX = fmodf(resultX , mSquareSize) / mSquareSize;
    float squareCoordY = fmodf(resultY , mSquareSize) / mSquareSize;

    float result;
    Vertex triangle[3];

    if(squareCoordX <= (1 - squareCoordY))
    {
        triangle[0] = mVertices[(gridY * mCol) + gridX];
        triangle[1] = mVertices[(gridY * mCol) + gridX + 1];
        triangle[2] = mVertices[(gridY * mCol) + gridX + mCol];

        Vector3D b = Vector2D(x, y).barycentricCoordinates(Vector2D(triangle[0].x, triangle[0].y),
                                                           Vector2D(triangle[1].x, triangle[1].y),
                                                           Vector2D(triangle[2].x, triangle[2].y));

        result = (triangle[0].z * b.x()) + (triangle[1].z * b.y()) + (triangle[2].z * b.z());
    }
    else
    {
        triangle[0] = mVertices[(gridY * mCol) + gridX + mCol + 1];
        triangle[1] = mVertices[(gridY * mCol) + gridX + mCol];
        triangle[2] = mVertices[(gridY * mCol) + gridX + 1];

        Vector3D b = Vector2D(x, y).barycentricCoordinates(Vector2D(triangle[0].x, triangle[0].y),
                                                           Vector2D(triangle[1].x, triangle[1].y),
                                                           Vector2D(triangle[2].x, triangle[2].y));

        result = (triangle[0].z * b.x()) + (triangle[1].z * b.y()) + (triangle[2].z * b.z());
    }

    return result;
}

void Terrain::makeTextureRepeat()
{
    for(unsigned int y = 0; y < mRow; y++)
    {
        for(unsigned int x = 0; x < mCol; x++)
        {
            mVertices[(y * mCol) + x].setUV(x, y);
        }
    }
}

void Terrain::makeTextureFill()
{
    for(unsigned int y = 0; y < mRow; y++)
    {
        for(unsigned int x = 0; x < mCol; x++)
        {
            mVertices[(y * mCol) + x].setUV(static_cast<float>(x) / static_cast<float>((mCol - 1)),
                                            static_cast<float>(y) / static_cast<float>((mRow - 1)));
        }
    }
}

void Terrain::createTerrain()
{
    for(unsigned int y = 0; y < mRow; y++)
    {
        for(unsigned int x = 0; x < mCol; x++)
        {
            mVertices.push_back(Vertex(x * mSquareSize, y * mSquareSize, rand() % 5,     0.f, 0.f, 1.f, (float) x / (float)(mCol - 1), (float) y / (float)(mRow - 1)));
        }
    }
}



