#ifndef GRID_H
#define GRID_H

#include "VisualObjects/visualobject.h"

class Grid : public VisualObject
{
    float mSquareSize;
    unsigned int mRow, mCol;

public:
    Grid();

    virtual void init() override;
    virtual void draw() override;

    void initDrawLines();
    void initDrawTriangles();
};

#endif // GRID_H
