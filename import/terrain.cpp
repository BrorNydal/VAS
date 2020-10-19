#include "terrain.h"

#include "globalconstants.h"

#include "Vendor/stb_image/stb_image.h"
#include <qdebug.h>

Terrain::Terrain(ETerrainType type, QVector2D size, unsigned int accuracy, const std::string &mapFile, float roughness)
    :   mRow(1), mCol(1), mTag(mapFile)
{
    switch(type)
    {
    case ETerrainType::square:
        createSquareTerrain(size);
        break;

    case ETerrainType::grid:
        createGridTerrain(size, accuracy);
        break;

    case ETerrainType::map:
        createMappedTerrain(size, accuracy, mapFile, roughness);
        break;

    default:
        break;
    }

    drawTriangles();
}

unsigned int *Terrain::getIndexData()
{
    return mIndices.data();
}

unsigned int Terrain::indexCount() const
{
    return mIndices.size();
}

Vertex *Terrain::getVertexData()
{
    return mVertices.data();
}

unsigned int Terrain::vertexCount() const
{
    return mVertices.size();
}

unsigned int Terrain::rowCount() const
{
    return mRow;
}

unsigned int Terrain::colCount() const
{
    return mCol;
}

std::string Terrain::getTag() const
{
    return mTag;
}

void Terrain::drawTriangles()
{
    mIndices.clear();

    for(unsigned int y = 0; y < mRow; y++)
    {
        for(unsigned int x = 0; x < mCol; x++)
        {
            //Lower Triangle
            mIndices.push_back( (y * (mCol+1)) + x                  );
            mIndices.push_back( (y * (mCol+1)) + (x + 1)            );
            mIndices.push_back( (y * (mCol+1)) + (x + 1) + mCol + 1 );

            //Upper Triangle
            mIndices.push_back( (y * (mCol+1)) + (x + 1) + mCol + 1 );
            mIndices.push_back( (y * (mCol+1)) + (x + 1) + mCol     );
            mIndices.push_back( (y * (mCol+1)) + x                  );
        }
    }
    qDebug() << "VERT SIZE : " << mVertices.size();

}

void Terrain::createSquareTerrain(QVector2D size)
{
    mVertices.clear();

    mCol = 1;
    mRow = 1;

    for(unsigned int y = 0; y < mRow + 1; y++)
    {
        for(unsigned int x = 0; x < mCol + 1; x++)
        {
            mVertices.push_back(Vertex((float)x * size.x(), (float)y * size.y(), 0, 0, 0, 1, (float) x / (float)(mCol - 1), (float) y / (float)(mRow - 1)));
        }
    }
}

void Terrain::createGridTerrain(QVector2D size, unsigned int accuracy)
{
    mVertices.clear();

    if(accuracy == 0)
    {
        for(unsigned int y = 0; y <= mRow; y++)
        {
            for(unsigned int x = 0; x <= mCol; x++)
            {
                mVertices.push_back(Vertex(((float)x * (float)size.x() / (float)mCol), ((float)y * (float)size.y() / (float)mRow) , 0, 0, 0, 1, (float) x / (float)(mCol), (float) y / (float)(mRow)));
            }
        }        
    }
    else
    {
        mCol = accuracy;
        mRow = accuracy;

        for(unsigned int y = 0; y < mRow + 1; y++)
        {
            for(unsigned int x = 0; x < mCol + 1; x++)
            {
                mVertices.push_back(Vertex(((float)x * (float)size.x() / (float)mCol), ((float)y * (float)size.y() / (float)mRow) , 0, 0, 0, 1, (float) x / (float)(mCol), (float) y / (float)(mRow)));
            }
        }
        mVertices[mCol*mRow-1].z = -1.f;
    }
}

void Terrain::createMappedTerrain(QVector2D size, unsigned int accuracy, const std::string &mapFile, float roughness)
{
    assert(accuracy > 0);

    mVertices.clear();

    mCol = accuracy;
    mRow = accuracy;

    unsigned char *data;
    int width, height, bpp;

    std::string file = FolderPath::HeightmapDirectory + mapFile;

    //STBI_grey
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(file.c_str(), &width, &height, &bpp, STBI_rgb);

    assert(data != nullptr && width > 0 && height > 0);

    mCol = width;
    mRow = height;

    createGridTerrain(size, 0);

    float maxDatavalue = 256.f;
    float limiter = maxDatavalue / (100.f - roughness);

    const unsigned int step = 3; //channels

    for(unsigned int y = 0; y < mRow; y++)
    {
        for(unsigned int x = 0; x <= mCol + 1; x++)
        {
            mVertices[(y * (mCol+1)) + x].z = (data[((y * mCol) + x) * step] / limiter);
        }
    }

    stbi_image_free(data);
}
