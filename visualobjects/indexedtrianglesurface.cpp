#include "indexedtrianglesurface.h"

#include "globalconstants.h"
#include "staticfunctions.h"

#include <fstream>
#include <QVector2D>

//ceilf
#include <cmath>
//fabs
#include <math.h>

TriangleSurface::TriangleSurface(std::string data, std::string index, float scale, bool las)
    :   mVertexFile(data), mIndexFile(index), mScale(scale), mIsLasFile(las), mObjectTriangleIndex(0)
{
    //mTag = "triangle surface";
    //mShaderType = EShader::plain;

}

void TriangleSurface::run()
{
    if(mIsLasFile)
    {
        readConvertedLasFile(mVertexFile);
        //calculateSurfaceNormal();
        calculateVertexNormals();
    }
    else
    {
        readCutsomVertexFile(mVertexFile);

        if(mIndexFile != "none")
        {
            readCustomIndexFile(mIndexFile);
            calculateSurfaceNormals();
            calculateVertexNormals();
        }
        else
        {

        }
    }
}

void TriangleSurface::draw(Shader &shader)
{
    shader.uniform3f("color", 0.f, 0.f, 1.f);
    shader.uniformf("maxz", mLimit.z.y() - mLimit.z.x());
    VisualObject::draw(shader);
}


void TriangleSurface::printDebugInformation()
{

}

void TriangleSurface::lasOptions(bool las, QVector2D triangleSize)
{
    mIsLasFile = las;
    mResolution = triangleSize;
}

SurfaceLimits TriangleSurface::findSurfaceLimit(std::string filename)
{
    std::ifstream file(Path::Datasets + filename + ".txt");

    //Fin the smallest x,y and z value
    if(file.is_open())
    {
        float v1, v2, v3;
        file >> v1 >> v2 >> v3;

        //Initialize the values as the first values found, and then search the rest
        mLimit.x = {v1, v1};
        mLimit.y = {v2, v2};
        mLimit.z = {v3, v3};

        while(!file.eof())
        {
            file >> v1 >> v2 >> v3;

            if(v1 < mLimit.x[0])
                mLimit.x[0] = v1;
            else if(v1 > mLimit.x[1])
                mLimit.x[1] = v1;

            if(v2 < mLimit.y[0])
                mLimit.y[0] = v2;
            else if(v2 > mLimit.y[1])
                mLimit.y[1] = v2;

            if(v3 < mLimit.z[0])
                mLimit.z[0] = v3;
            else if(v3 > mLimit.z[1])
                mLimit.z[1] = v3;
        }

        file.close();
    }

    return mLimit;
}

void TriangleSurface::readConvertedLasFile(std::string filename)
{
    std::ifstream file(Path::Datasets + filename + ".txt");

    findSurfaceLimit(filename);

    float lengthX = fabs(mLimit.offsetMaxX()) - fabs(mLimit.offsetMinX());
    float lengthY = fabs(mLimit.offsetMaxY()) - fabs(mLimit.offsetMinY());
    float deltaX = lengthX / mResolution.x();
    float deltaY = lengthY / mResolution.y();

    mCol = lengthX / deltaX;
    mRow = lengthY / deltaY;

    //create grid
    for(unsigned int y = 0; y <= mRow; y++)
    {
        for(unsigned int x = 0; x <= mCol; x++)
        {
            mVertices.push_back(Vertex((float)x * deltaX, (float)y * deltaY, 0.f, 0.f, 0.f, 1.f, (float) x, (float) y));
            mHeightData[std::pair<unsigned int, unsigned int>(x, y)] = std::pair<unsigned int, float>();
        }
    }

//    mCol = 5;
//    mRow = 5;

//    for(unsigned int y = 0; y <= mRow; y++)
//    {
//        for(unsigned int x = 0; x <= mCol; x++)
//        {
//            mVertices.push_back(Vertex((float)x * deltaX, (float)y * deltaY, 0.f, 0.f, 0.f, 1.f, (float) x, (float) y));
//        }
//    }

//    mVertices[vertexIndex(5, 0)].z = 20.f;
//    mVertices[vertexIndex(4, 0)].z = 10.f;
//    mVertices[vertexIndex(3, 5)].z = 10.f;
//    mVertices[vertexIndex(4, 5)].z = 20.f;
//    mVertices[vertexIndex(0, 3)].z = 10.f;

    mSquareSize[0] = deltaX;
    mSquareSize[1] = deltaY;
    mTotalSize[0] = deltaX * (float)mCol;
    mTotalSize[1] = deltaY * (float)mRow;

    if(deltaX >= deltaY)
        mHeightAcceptanceRadius = deltaX;
    else
        mHeightAcceptanceRadius = deltaY;

    //float testx = 22.f;
    //float testy = 26.f;
//    qDebug() << "square at :" << testx << testy;
//    qDebug() << "result :" << getSquare(testx, testy);    

    //read las file, fetch height data
    if (file.is_open())
    {
        while(!file.eof())
        {
            //read three floats from file
            float v1, v2, v3;
            file >> v1 >> v2 >> v3;

            v1 += mLimit.offsetMinX();
            v2 += mLimit.offsetMinY();
            v3 += mLimit.offsetMaxZ();

            if(v1 >= 0.f && v1 <= mTotalSize.x() && v2 >= 0.f && v2 <= mTotalSize.y())
            {
                QPoint square = getBottomLeftVertaxAtSquare(v1, v2);

                //give data to each vertex in square

                //bottom left
                if((QVector2D(v1, v2) - QVector2D(mVertices[vertexIndex(square)].x, mVertices[vertexIndex(square)].y)).length() < mHeightAcceptanceRadius)
                {
                    mHeightData[std::pair<unsigned int, unsigned int>(square.x(), square.y())].first++;
                    mHeightData[std::pair<unsigned int, unsigned int>(square.x(), square.y())].second += v3;
                }

                //bottom right
                if((QVector2D(v1, v2) - QVector2D(mVertices[vertexIndex(square.x() + 1, square.y())].x, mVertices[vertexIndex(square.x() + 1, square.y())].y)).length() < mHeightAcceptanceRadius)
                {
                    mHeightData[std::pair<unsigned int, unsigned int>(square.x() + 1, square.y())].first++;
                    mHeightData[std::pair<unsigned int, unsigned int>(square.x() + 1, square.y())].second += v3;
                }

                //top left
                if((QVector2D(v1, v2) - QVector2D(mVertices[vertexIndex(square.x(), square.y() + 1)].x, mVertices[vertexIndex(square.x(), square.y() + 1)].y)).length() < mHeightAcceptanceRadius)
                {
                    mHeightData[std::pair<unsigned int, unsigned int>(square.x(), square.y() + 1)].first++;
                    mHeightData[std::pair<unsigned int, unsigned int>(square.x(), square.y() + 1)].second += v3;
                }

                //top right
                if((QVector2D(v1, v2) - QVector2D(mVertices[vertexIndex(square.x() + 1, square.y() + 1)].x, mVertices[vertexIndex(square.x() + 1, square.y() + 1)].y)).length() < mHeightAcceptanceRadius)
                {
                    mHeightData[std::pair<unsigned int, unsigned int>(square.x() + 1, square.y() + 1)].first++;
                    mHeightData[std::pair<unsigned int, unsigned int>(square.x() + 1, square.y() + 1)].second += v3;
                }

                //mVertices[vertexIndex(square)].z = v3;
            }
        }

        file.close();        
    }
    else
    {
        qDebug() << "(indexedtrianglesurface) Can't read datafile!";
    }

    for(unsigned int y = 0; y < mRow; y++)
    {
        for(unsigned int x = 0; x < mCol; x++)
        {
            const unsigned int count = mHeightData[std::pair<unsigned int, unsigned int>(x, y)].first;
            const float total = mHeightData[std::pair<unsigned int, unsigned int>(x, y)].second;

            if(count > 0)
                mVertices[vertexIndex(x, y)].z = total / count;
            else
                mVertices[vertexIndex(x, y)].z = 0.f;
        }
    }

    assertIndices();
}

void TriangleSurface::assertIndices()
{
    unsigned int lti[3];
    unsigned int uti[3];

//    mSquares.clear();
//    mSquares.shrink_to_fit();
    mTriangles.clear();

    mSquares.resize(vertexIndex(mCol-1, mRow-1) + 1);

    for(unsigned int y = 0; y < mRow; y++)
    {
        for(unsigned int x = 0; x < mCol; x++)
        {
            //Lower Triangle

            lti[0] = vertexIndex(x, y);
            lti[1] = vertexIndex(x+1, y);
            lti[2] = vertexIndex(x, y+1);

            mIndices.push_back(lti[0]);
            mIndices.push_back(lti[1]);
            mIndices.push_back(lti[2]);

            Square square;

            square.lower.mIndices[0] = lti[0];
            square.lower.mIndices[1] = lti[1];
            square.lower.mIndices[2] = lti[2];

            square.lower.mSurfaceNormal = CalculateTriangleNormal(mVertices[lti[0]], mVertices[lti[1]], mVertices[lti[2]]);
            square.lower.mSurfaceNormal.normalize();

            mTriangles.push_back(square.lower);

            //Upper Triangle

            uti[0] = vertexIndex(x+1, y+1);
            uti[1] = vertexIndex(x, y+1);
            uti[2] = vertexIndex(x+1, y);

            mIndices.push_back(uti[0]);
            mIndices.push_back(uti[1]);
            mIndices.push_back(uti[2]);

            square.upper.mIndices[0] = uti[0];
            square.upper.mIndices[1] = uti[1];
            square.upper.mIndices[2] = uti[2];

            square.upper.mSurfaceNormal = CalculateTriangleNormal(mVertices[uti[0]], mVertices[uti[1]], mVertices[uti[2]]);
            square.upper.mSurfaceNormal.normalize();

            mTriangles.push_back(square.upper);
            mSquares[vertexIndex(x, y)] = square;
        }
    }
}

QPoint TriangleSurface::getBottomLeftVertaxAtSquare(float x, float y) const
{
    //Where we are relative to the terrain
    float resultX = x;// + mTotalSize.x() / 2.f;
    float resultY = y;// + mTotalSize.y() / 2.f;

    //Which square we are standing on
    int gridX = (int) floorf(resultX / mSquareSize.x());
    int gridY = (int) floorf(resultY / mSquareSize.y());

    return QPoint(gridX, gridY);
}

Triangle *TriangleSurface::getTriangle(float x, float y)
{
    int index = vertexIndex(getBottomLeftVertaxAtSquare(x, y));

    if(index >= 0 && (unsigned int)index < mSquares.size())
    {
        if(mSquares[index].onUpper)
            return &mSquares[index].upper;
        else
            return &mSquares[index].lower;
    }
    else
    {
        //qDebug() << "[WARNING] (trianglesurface) gettriangle(x,y) square nullptr.";
        //qDebug() << "square index : " << index;
        //qDebug() << "square vector size :" << mSquares.size();
        return nullptr;
    }
}

void TriangleSurface::readCutsomVertexFile(std::string filename)
{
    std::ifstream file(Path::Datasets + filename + ".txt");

    findSurfaceLimit(filename);

    if (file.is_open())
    {
        //If we successfully open the new file, we clear the existing vertices
        mVertices.clear();

        //file while-loop count
        unsigned int count = 0;

        while(!file.eof())
        {
            //read three floats from file
            float v1, v2, v3;
            file >> v1 >> v2 >> v3;

            mVertices.push_back(Vertex((v1 + mLimit.offsetMinX()) * mScale, (v2 + mLimit.offsetMinY()) * mScale, (v3 + mLimit.offsetMaxZ()) * mScale));

            count++;
        }

        file.close();
    }
    else
    {
        qDebug() << "(indexedtrianglesurface) Can't read datafile!";
    }

    qDebug() << "surface size :" << mVertices.size();
}

void TriangleSurface::readCustomIndexFile(std::string filename)
{
    std::ifstream file(Path::Datasets + filename + ".txt");

    if (file.is_open())
    {
        //If we successfully open the new file, we clear the existing vertices
        mIndices.clear();

        unsigned int count = 0; //while-loop count

        while(!file.eof())
        {
            unsigned int v1, v2, v3;
            file >> v1 >> v2 >> v3;            

            mIndices.push_back(v1);
            mIndices.push_back(v2);
            mIndices.push_back(v3);

            Triangle t;
            t.mIndices[0] = v1;
            t.mIndices[1] = v2;
            t.mIndices[2] = v3;

            unsigned int i1, i2, i3;
            file >> i1 >> i2 >> i3;

            t.mNeighbours[0] = i1;
            t.mNeighbours[1] = i2;
            t.mNeighbours[2] = i3;

            mTriangles.push_back(t);

            count++;
        }

        file.close();
    }
    else{
        qDebug() << "(indexedtrianglesurface) Can't read indexfile!";
    }
}



float TriangleSurface::heightAtLocation(float x, float y)
{
    if(!mIsLasFile)
        return barycentricHeightSearch(QVector2D(x, y));

    //Where we are relative to the terrain
    //need to apply offset, atm only works when vertex[0] starts at (0,0)
    float resultX = x;// + mTotalSize.x() / 2.f;
    float resultY = y;// + mTotalSize.y() / 2.f;

    //Which square we are standing on
    int gridX = (int) floorf(resultX / mSquareSize.x());
    int gridY = (int) floorf(resultY / mSquareSize.y());

    //qDebug() << gridX << gridY;

    if((unsigned int)gridX >= mCol || (unsigned int)gridY >= mRow || gridX < 0 || gridY < 0)
    {
        return 0.f;
    }

    float squareCoordX = fmodf(resultX , mSquareSize.x()) / mSquareSize.x();
    float squareCoordY = fmodf(resultY , mSquareSize.y()) / mSquareSize.y();

    float result;
    Vertex triangle[3];

    //triangle vertex indices
    unsigned int iv0;
    unsigned int iv1;
    unsigned int iv2;

    //lower left triangle
    if(squareCoordX <= (1 - squareCoordY))
    {
        iv0 = vertexIndex(gridX, gridY);//row(gridY) + col(gridX);
        iv1 = vertexIndex(gridX+1, gridY);//row(gridY) + col(gridX+1);
        iv2 = vertexIndex(gridX, gridY+1);//row(gridY) + row(1) + col(gridX);

        mObjectSquareIndex = vertexIndex(gridX, gridY);

        if(mObjectSquareIndex >= 0 && mObjectSquareIndex < mSquares.size())
            mSquares[mObjectSquareIndex].onUpper = false;

        triangle[1] = mVertices[iv0];
        triangle[2] = mVertices[iv1];
        triangle[0] = mVertices[iv2];

        QVector3D b = BarycentricCoordinates(QVector2D(resultX, resultY),
                                             QVector2D(triangle[0].x, triangle[0].y),
                                             QVector2D(triangle[1].x, triangle[1].y),
                                             QVector2D(triangle[2].x, triangle[2].y));

        result = (triangle[0].z * b.x()) + (triangle[1].z * b.y()) + (triangle[2].z * b.z());
    }

    //upper right triangle
    else
    {
        iv0 = vertexIndex(gridX+1, gridY+1);//row(gridY) + row(1) + col(gridX+1);
        iv1 = vertexIndex(gridX, gridY+1);  //row(gridY) + row(1) + col(gridX);
        iv2 = vertexIndex(gridX+1, gridY);  //row(gridY) + col(gridX+1);

        mObjectSquareIndex = vertexIndex(gridX, gridY);

        if(mObjectSquareIndex >= 0 && mObjectSquareIndex < mSquares.size())
            mSquares[mObjectSquareIndex].onUpper = true;

        triangle[1] = mVertices[iv0];
        triangle[2] = mVertices[iv1];
        triangle[0] = mVertices[iv2];

        QVector3D b = BarycentricCoordinates(QVector2D(resultX, resultY),
                                             QVector2D(triangle[0].x, triangle[0].y),
                                             QVector2D(triangle[1].x, triangle[1].y),
                                             QVector2D(triangle[2].x, triangle[2].y));


        result = (triangle[0].z * b.x()) + (triangle[1].z * b.y()) + (triangle[2].z * b.z());
    }

    return result;
}

float TriangleSurface::barycentricHeightSearch(QVector2D loc)
{
    QVector3D t[3];

    //qDebug() << "triangle index :" << mObjectTriangleIndex;

    for(unsigned int i = 0; i < 3; i++)
    {
        //qDebug() << "Triangle" << i << getCurrentTriangle().mPosition[i];
        t[i] = mVertices[ getCurrentTriangle().mIndices[i] ];
    }

    //qDebug() << "LOC :" << loc;
    QVector3D bc = BarycentricCoordinates(loc, QVector2D(t[0].x(), t[0].y()),
                                               QVector2D(t[1].x(), t[1].y()),
                                               QVector2D(t[2].x(), t[2].y()));

    //qDebug() << "BARYC :" << bc;

    //on triangle
    if(bc.x() >= 0.f && bc.y() >= 0.f && bc.z() >= 0.f)
    {
        return (t[0].z() * bc.x() + t[1].z() * bc.y() + t[2].z() * bc.z());
    }
    //outside triangle
    else
    {
        for(unsigned int i = 0; i < 3; i++)
        {
            if(bc[i] < 0.f)
            {
                if(getCurrentTriangle().mNeighbours[i] != -1)
                {
                    //qDebug() << "OPP " << i << "," << getCurrentTriangle().mAdjacentTriangles[i];
                    mObjectTriangleIndex = getCurrentTriangle().mNeighbours[i];
                    break;
                }
                else
                {
                    //qDebug() << "INVALID " << i;
                    return (t[0].z() * bc.x() + t[1].z() * bc.y() + t[2].z() * bc.z());
                }
            }
        }

        return barycentricHeightSearch(loc);
    }
}

const Triangle &TriangleSurface::getCurrentTriangle() const
{
    return mTriangles[mObjectTriangleIndex];
}

const Square &TriangleSurface::getCurrentSquare(float x, float y) const
{
    return mSquares[vertexIndex(getBottomLeftVertaxAtSquare(x, y))];
}

unsigned TriangleSurface::vertexIndex(QPoint point) const
{
    return vertexIndex(point.x(), point.y());
}

unsigned TriangleSurface::vertexIndex(unsigned int x, unsigned int y) const
{
    return row(y) + col(x);
}

unsigned int TriangleSurface::row(unsigned int i) const
{
    return i * (mCol+1);
}

unsigned int TriangleSurface::col(unsigned int i) const
{
    return i;
}

void TriangleSurface::calculateSurfaceNormals()
{
    for(auto it = mTriangles.begin(); it != mTriangles.end(); it++)
    {
        QVector3D temp1 = QVector3D(mVertices[(*it).mIndices[0]].x, mVertices[(*it).mIndices[0]].y, mVertices[(*it).mIndices[0]].z);
        QVector3D temp2 = QVector3D(mVertices[(*it).mIndices[1]].x, mVertices[(*it).mIndices[1]].y, mVertices[(*it).mIndices[1]].z);
        QVector3D temp3 = QVector3D(mVertices[(*it).mIndices[2]].x, mVertices[(*it).mIndices[2]].y, mVertices[(*it).mIndices[2]].z);

        QVector3D surfaceNormal;

        //Calculate the relative surface normal using temporary vectors
        //In all cases with all triangles pointing positive in one direction, we can measure the respective axis-value recieved
        if(CalculateTriangleNormal(temp1, temp2, temp3).z() > 0) //All surfaces are pointing in positive z-direction
            surfaceNormal = CalculateTriangleNormal(temp1, temp2, temp3);
        else
            surfaceNormal = CalculateTriangleNormal(temp1, temp3, temp2);

        surfaceNormal.normalize();
        (*it).mSurfaceNormal = surfaceNormal;

        //While we already have its position, grab its centroid
        //(*it).mCentroid = MyMathFunctions::centroidOfTriangle(temp1, temp2, temp3);
    }
}

void TriangleSurface::calculateVertexNormals()
{
    //For each vertex we calculate average normals of surrounding surfaces
    for(unsigned int i = 0; i < mVertices.size(); i++)
    {
        std::vector<QVector3D> surfaceNormals;

        //For each triangle we look at its indecies
        for(unsigned int j = 0; j < mTriangles.size(); j++)
        {
            //Each triangle index will be between 0 and mVertices.size()
            //And atleast one triangle will have an index equal to i
            for(unsigned int k = 0; k < 3; k++)
            {
                if(mTriangles[j].mIndices[k] == (int)i)
                {
                    //When we find an index equal to i, we add it to the list of surface normals for further calculation
                    surfaceNormals.push_back(mTriangles[j].mSurfaceNormal);
                    break;
                }
            }
        }

        if(surfaceNormals.size() == 0)
            qDebug() << "[WARNING] Found no index corresponding to vertex in calculateNormals() \n" << "Vertex normal will be set to 0, 0, 0";

        //Calculate vertex normal and set current vertex normals to it
        QVector3D averageNormal = MyMathFunctions::average3DVector(surfaceNormals);
        averageNormal.normalize();
        mVertices[i].setRGB(averageNormal.x(), averageNormal.y(), averageNormal.z());
    }
}
