#ifndef TERRAIN_H
#define TERRAIN_H

#include "VisualObjects/visualobject.h"
#include <QMatrix>

class Terrain : public VisualObject
{
public:
    Terrain();
    Terrain(unsigned int col, unsigned int row, float squareSize = 1.f);

    virtual void init() override;
    virtual void draw() override;

    void initializeDrawGLLines();
    void initializeDrawGLTriangles();

    GLenum getDrawMode() {return mDrawMode;}
    float getHeightAtPosition(float x, float y);

    void makeTextureRepeat();
    void makeTextureFill();

protected:

    //How many triangles in height and width
    const unsigned int mCol, mRow;
    //Height and width of each triangle
    const float mSquareSize = 8.f;
    const unsigned int mTerrainSize;

    std::vector< Vector3D > mTriangles;
    QVector<float> mHeightMap;
    int mOffsetX;
    int mOffsetY;

    //QVector<float> mTerrainHeightMap;

private:
    void createTerrain();
};

#endif // TERRAIN_H
