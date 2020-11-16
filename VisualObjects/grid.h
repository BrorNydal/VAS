#ifndef GRID_H
#define GRID_H

#include "VisualObjects/visualobject.h"

class Grid : public VisualObject
{
    float mSquareSize;
    unsigned int mRow, mCol;

public:
    Grid();

    void initDrawLines();
    void initDrawTriangles();
};

#endif // GRID_H
