#include "grid.h"

Grid::Grid()
    :   mSquareSize(1.f), mRow(20), mCol(20)
{
    for(unsigned int y = 0; y < mRow; y++)
    {
        for(unsigned int x = 0; x < mCol; x++)
        {
            mVertices.push_back(Vertex(x * mSquareSize, y * mSquareSize, 0.f,    0.f, 0.f, 0.f));
        }
    }

    initDrawLines();
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

    mDrawMode = GL_LINES;
    mDrawArrays = false;
}

