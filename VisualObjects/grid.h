#ifndef GRID_H
#define GRID_H

#include "VisualObjects/visualobject.h"

class Grid : public VisualObject
{
private:
    const float mSquareSize;
    const unsigned int mRow, mCol;

public:
    Grid();
    Grid(unsigned int row, unsigned int col, float squareSize = 1.f);

    virtual void init() override;
    virtual void draw() override;

    void initDrawLines();
    void initDrawTriangles();

private:
    void createGrid();
};

#endif // GRID_H
