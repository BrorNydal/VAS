#ifndef TERRAIN_H
#define TERRAIN_H

#include <QVector2D>
#include "enumclasses.h"
#include "vertex.h"
#include <vector>

class Texture;

class Terrain
{
    ETerrainType mType;
    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;

    unsigned int mRow, mCol;
    const std::string mTag;

public:
    Terrain(ETerrainType type, QVector2D size, unsigned int accuracy = 0, const std::string &mapFile = "", float rougness = 1.f);

    unsigned int *getIndexData();
    unsigned int indexCount() const;
    Vertex *getVertexData();
    unsigned int vertexCount() const;

    unsigned int rowCount() const;
    unsigned int colCount() const;

    std::string getTag() const;

    void drawTriangles();

private:

    void createSquareTerrain(QVector2D size);
    void createGridTerrain(QVector2D size, unsigned int accuracy);
    void createMappedTerrain(QVector2D size, unsigned int accuracy, const std::string &mapFile, float roughness);

};

#endif // TERRAIN_H
