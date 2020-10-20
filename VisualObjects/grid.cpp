#include "grid.h"

Grid::Grid()
    :   mSquareSize(1.f), mRow(9), mCol(9)
{
    for(unsigned int y = 0; y < mRow; y++)
    {
        for(unsigned int x = 0; x < mCol; x++)
        {
            mVertices.push_back(Vertex(x * mSquareSize, y * mSquareSize, 0.f,    0.f, 0.f, 0.f));
        }
    }
}

void Grid::init()
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

void Grid::draw()
{
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.data());
    glBindVertexArray( mVAO );
    glDrawElements(GL_LINES, static_cast<int>(mIndices.size()), GL_UNSIGNED_INT, NULL);
}

void Grid::initDrawLines()
{
    mIndices.clear();

    for(unsigned int y = 0; y < mRow; y++)
    {
        for(unsigned int x = 0; x < mCol; x++)
        {
            if(x != mCol - 1 && y != 0) //y != 0 when displayed with xyz
            {
                mIndices.push_back((y * mCol) + x);
                mIndices.push_back((y * mCol) + x + 1);
            }

            if(y != mRow - 1 && x != 0) //x != 0 when displayed with xyz
            {
                mIndices.push_back((y * mCol) + x);
                mIndices.push_back((y * mCol) + x + mCol);
            }
        }
    }
}

void Grid::initDrawTriangles()
{
    mDrawMode = GL_TRIANGLES;

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
