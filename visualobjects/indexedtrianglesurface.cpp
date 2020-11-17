#include "indexedtrianglesurface.h"

#include "globalconstants.h"
#include "staticfunctions.h"
#include "mymathfunctions.h"

#include <fstream>
#include <QVector2D>

//ceilf
#include <cmath>
//fabs
#include <math.h>

IndexedTriangleSurface::IndexedTriangleSurface(std::string data, std::string index, float scale, bool las)
    :   mVertexFile(data), mIndexFile(index), mScale(scale), mIsLasFile(las), mObjectTriangleIndex(0)
{
    mTag = "triangle surface";


}

void IndexedTriangleSurface::run()
{
    if(mIsLasFile)
    {
        readConvertedLasFile(mVertexFile);
        calculateSurfaceNormal();
        calculateVertexNormal();
    }
    else
    {
        readCutsomFile(mVertexFile);

        if(mIndexFile != "none")
        {
            readCustomIndexFile(mIndexFile);
            calculateSurfaceNormal();
            calculateVertexNormal();
            writeFile();
        }
        else
        {

        }
    }
}

void IndexedTriangleSurface::draw(Shader &shader)
{
    shader.uniform3f("color", 4.f, 2.f, 3.f);
    VisualObject::draw(shader);
}


void IndexedTriangleSurface::printDebugInformation()
{

}

void IndexedTriangleSurface::lasOptions(bool las, QVector2D triangleSize)
{
    mIsLasFile = las;
    mResolution = triangleSize;
}

SurfaceLimits IndexedTriangleSurface::findSurfaceLimit(std::string filename)
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

void IndexedTriangleSurface::readConvertedLasFile(std::string filename)
{
    std::ifstream file(Path::Datasets + filename + ".txt");

    findSurfaceLimit(filename);

    float lengthX = fabs(mLimit.offsetMaxX()) - fabs(mLimit.offsetMinX());
    float lengthY = fabs(mLimit.offsetMaxY()) - fabs(mLimit.offsetMinY());
    float deltaX = lengthX / mResolution.x();
    float deltaY = lengthY / mResolution.y();

    qDebug() << "length x :" << lengthX;
    qDebug() << "length y :" << lengthY;
    qDebug() << "delta x :" << deltaX;
    qDebug() << "delta y :" << deltaY;

    mCol = lengthX / deltaX;
    mRow = lengthY / deltaY;

    //create grid
    for(unsigned int y = 0; y <= mRow; y++)
    {
        for(unsigned int x = 0; x <= mCol; x++)
        {
            mVertices.push_back(Vertex((float)x * deltaX, (float)y * deltaY, 0.f, 0.f, 0.f, 1.f, (float) x, (float) y));
        }
    }

    //mCol = 7;
    //mRow = 7;

//    for(unsigned int y = 0; y <= mRow; y++)
//    {
//        qDebug() << "y :" << y;

//        for(unsigned int x = 0; x <= mCol; x++)
//        {
//            qDebug() << "x :" << x;
//            mVertices.push_back(Vertex((float)x * 2.f, (float)y * 2.f, 0.f, 0.f, 0.f, 1.f, (float) x, (float) y));
//        }
//    }

    //mVertices[5*(mCol+1) + 5].z = 20.f;

    mSquareSize[0] = deltaX;
    mSquareSize[1] = deltaY;
    mTotalSize[0] = deltaX * (float)mCol;
    mTotalSize[1] = deltaY * (float)mRow;

    qDebug() << "total size :" << mTotalSize;
    //float testx = 22.f;
    //float testy = 26.f;
//    qDebug() << "square at :" << testx << testy;
//    qDebug() << "result :" << getSquare(testx, testy);

    assertIndices();

    //read las file
    if (file.is_open())
    {
        //file while-loop count
        unsigned int count = 0;

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
                QPoint square = getSquare(v1, v2);
                mVertices[square.y() * mCol + square.x()].z = v3;
            }
//            mLasData.push_back(QVector3D((v1 + mLimit.offsetMinX()) * mScale, (v2 + mLimit.offsetMinY()) * mScale, (v3 + mLimit.offsetMaxZ()) * mScale));
//            mIndices.push_back(count);

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

void IndexedTriangleSurface::assertIndices()
{
    unsigned int lti[3];
    unsigned int uti[3];

    unsigned int tc = 0;
    for(unsigned int y = 0; y < mRow; y++)
    {
        for(unsigned int x = 0; x < mCol; x++)
        {
            //Lower Triangle

            lti[0] = y * (mCol+1) + x;
            lti[1] = y * (mCol+1) + x + 1;
            lti[2] = y * (mCol+1) + x + mCol + 1;

            mIndices.push_back(lti[0]);
            mIndices.push_back(lti[1]);
            mIndices.push_back(lti[2]);

            Triangle ltriangle(tc);
            ltriangle.mIndices[0] = lti[0];
            ltriangle.mIndices[1] = lti[1];
            ltriangle.mIndices[2] = lti[2];
            mTriangles.push_back(ltriangle);
            tc++;

            //Upper Triangle

            uti[0] =  y * (mCol+1) + x + mCol + 2;
            uti[1] =  y * (mCol+1) + x + mCol + 1;
            uti[2] =  y * (mCol+1) + x + 1;

            mIndices.push_back(uti[0]);
            mIndices.push_back(uti[1]);
            mIndices.push_back(uti[2]);

            Triangle utriangle(tc);
            utriangle.mIndices[0] = uti[0];
            utriangle.mIndices[1] = uti[1];
            utriangle.mIndices[2] = uti[2];
            mTriangles.push_back(utriangle);
            tc++;
        }
    }
}

QPoint IndexedTriangleSurface::getSquare(float x, float y)
{
    //Where we are relative to the terrain
    float resultX = x;// + mTotalSize.x() / 2.f;
    float resultY = y;// + mTotalSize.y() / 2.f;

    //Which square we are standing on
    int gridX = (int) floorf(resultX / mSquareSize.x());
    int gridY = (int) floorf(resultY / mSquareSize.y());

    return QPoint(gridX, gridY);
}

void IndexedTriangleSurface::readCutsomFile(std::string filename)
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

void IndexedTriangleSurface::readCustomIndexFile(std::string filename)
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

            Triangle t(count);
            t.mIndices[0] = v1;
            t.mIndices[1] = v2;
            t.mIndices[2] = v3;

//            t.mPosition[0] = mVertices[v1];
//            t.mPosition[1] = mVertices[v2];
//            t.mPosition[2] = mVertices[v3];

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

void IndexedTriangleSurface::writeFile()
{
    std::string vertexFileName = "Oppg_5_2_11_VertexData";
    std::ofstream vertexFile(Path::Datasets + vertexFileName + ".txt");

    if(vertexFile.is_open())
    {
        //Creates new file or overrides an existing file with same path
        for(unsigned int i = 0; i < mVertices.size(); i++)
        {
            vertexFile << mVertices[i] << "\n";
        }
    }else{
        qDebug() << "(indexedtrianglesurface) Can't write data file!";
    }

    vertexFile.close();

    std::string indexFileName = "Oppg_5_2_11_IndexData";
    std::ofstream indexFile(Path::Datasets + indexFileName + ".txt");

    if(indexFile.is_open())
    {
        unsigned triangleIndex = 0;

        //Creates new file or overrides an existing file with same path
        for(unsigned int i = 0; i < mIndices.size(); i+=3)
        {
            indexFile << mIndices[i] << " " << mIndices[i+1] << " " << mIndices[i+2] << "   ";
            indexFile << mTriangles[triangleIndex].mNeighbours[0] << " " <<
                         mTriangles[triangleIndex].mNeighbours[1] << " " <<
                         mTriangles[triangleIndex].mNeighbours[2];

            indexFile << "\n";
            triangleIndex++;
        }
    }else
    {
        qDebug() << "(indexedtrianglesurface) Can't write index file!";
    }

    vertexFile.close();
}

float IndexedTriangleSurface::heightAtLocation(float x, float y)
{
    //Where we are relative to the terrain
    //need to apply offset, atm only works when vertex[0] starts at (0,0)
    float resultX = x;// + mTotalSize.x() / 2.f;
    float resultY = y;// + mTotalSize.y() / 2.f;

    //Which square we are standing on
    int gridX = (int) floorf(resultX / mSquareSize.x());
    int gridY = (int) floorf(resultY / mSquareSize.y());

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
        iv0 = (gridY * (mCol+1)) + gridX                 ;
        iv1 = (gridY * (mCol+1)) + (gridX + 1)           ;
        iv2 = (gridY * (mCol+1)) + (gridX + 1) + mCol ;

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
        iv0 = gridY * (mCol+1) + gridX + mCol + 2;
        iv1 = gridY * (mCol+1) + gridX + mCol + 1;
        iv2 = gridY * (mCol+1) + gridX + 1;

        triangle[1] = mVertices[iv0];
        triangle[2] = mVertices[iv1];
        triangle[0] = mVertices[iv2];

        QVector3D b = BarycentricCoordinates(QVector2D(resultX, resultY),
                                             QVector2D(triangle[0].x, triangle[0].y),
                                             QVector2D(triangle[1].x, triangle[1].y),
                                             QVector2D(triangle[2].x, triangle[2].y));


        result = (triangle[0].z * b.x()) + (triangle[1].z * b.y()) + (triangle[2].z * b.z());
    }

    qDebug() << "HERE1";
    return result;
}

float IndexedTriangleSurface::barycentricHeightSearch(QVector2D loc)
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

const Triangle &IndexedTriangleSurface::getCurrentTriangle() const
{
    return mTriangles[mObjectTriangleIndex];
}

void IndexedTriangleSurface::calculateSurfaceNormal()
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

void IndexedTriangleSurface::calculateVertexNormal()
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
                if(mTriangles[j].mIndices[k] == i)
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
        mVertices[i].setRGB(averageNormal.x(), averageNormal.y(), averageNormal.z());
    }
}
