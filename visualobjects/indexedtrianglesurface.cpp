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
    qDebug() << "TRIANGLE CENTROIDS:";
    for(unsigned int i = 0; i < mTriangles.size(); i++)
        qDebug() << "Triangle" << i << ":" << mTriangles[i].mCentroid.x() << mTriangles[i].mCentroid.y() << mTriangles[i].mCentroid.z();

}

void IndexedTriangleSurface::lasOptions(bool las, QVector2D size)
{
    mIsLasFile = las;
    mResolution = size;
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

    qDebug() << "resolution x" << mResolution.x();
    qDebug() << "resolution y" << mResolution.y();

    qDebug() << "limit x :" << mLimit.x.y() - mLimit.x.x();
    qDebug() << "limit y :" << mLimit.y.y() - mLimit.y.x();

    qDebug() << "size of file x" << ceilf((fabs(mLimit.offsetMinX()) - fabs(mLimit.offsetMaxX())));
    qDebug() << "size of file y" << ceilf((fabs(mLimit.offsetMinY()) - fabs(mLimit.offsetMaxY())));

    mCol = ceilf((fabs(mLimit.offsetMaxX()) - fabs(mLimit.offsetMinX())) / mResolution.x());
    mRow = ceilf((fabs(mLimit.offsetMaxY()) - fabs(mLimit.offsetMinY())) / mResolution.y());

    qDebug() << "col :" << mCol;
    qDebug() << "row :" << mRow;

    for(unsigned int y = 0; y < mRow; y++)
    {
        for(unsigned int x = 0; x < mCol; x++)
        {
            mVertices.push_back(Vertex((float)x * (float)mResolution.x(), (float)y * (float)mResolution.y() , 0, 0, 0, 1, (float) x, (float) y));
        }
    }

    mTotalSize[0] = (float)mResolution.x() * (float)col;
    mTotalSize[1] = (float)mResolution.y() * (float)row;

    qDebug() << "size :" << mTotalSize;

    assertIndices();

    if (file.is_open())
    {
        //file while-loop count
        unsigned int count = 0;

        while(!file.eof())
        {
            //read three floats from file
            float v1, v2, v3;
            file >> v1 >> v2 >> v3;

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
            t.mIndecies[0] = v1;
            t.mIndecies[1] = v2;
            t.mIndecies[2] = v3;

            t.mPosition[0] = mVertices[v1];
            t.mPosition[1] = mVertices[v2];
            t.mPosition[2] = mVertices[v3];

            unsigned int i1, i2, i3;
            file >> i1 >> i2 >> i3;

            t.mAdjacentTriangles[0] = i1;
            t.mAdjacentTriangles[1] = i2;
            t.mAdjacentTriangles[2] = i3;

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
            indexFile << mTriangles[triangleIndex].mAdjacentTriangles[0] << " " <<
                         mTriangles[triangleIndex].mAdjacentTriangles[1] << " " <<
                         mTriangles[triangleIndex].mAdjacentTriangles[2];

            indexFile << "\n";
            triangleIndex++;
        }
    }else
    {
        qDebug() << "(indexedtrianglesurface) Can't write index file!";
    }

    vertexFile.close();
}

float IndexedTriangleSurface::barycentricHeightSearch(QVector2D loc)
{
    QVector3D t[3];

    //qDebug() << "triangle index :" << mObjectTriangleIndex;

    for(unsigned int i = 0; i < 3; i++)
    {
        //qDebug() << "Triangle" << i << getCurrentTriangle().mPosition[i];
        t[i] = getCurrentTriangle().mPosition[i];
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
                if(getCurrentTriangle().mAdjacentTriangles[i] != -1)
                {
                    //qDebug() << "OPP " << i << "," << getCurrentTriangle().mAdjacentTriangles[i];
                    mObjectTriangleIndex = getCurrentTriangle().mAdjacentTriangles[i];
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
        QVector3D temp1 = QVector3D(mVertices[(*it).mIndecies[0]].x, mVertices[(*it).mIndecies[0]].y, mVertices[(*it).mIndecies[0]].z);
        QVector3D temp2 = QVector3D(mVertices[(*it).mIndecies[1]].x, mVertices[(*it).mIndecies[1]].y, mVertices[(*it).mIndecies[1]].z);
        QVector3D temp3 = QVector3D(mVertices[(*it).mIndecies[2]].x, mVertices[(*it).mIndecies[2]].y, mVertices[(*it).mIndecies[2]].z);

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
        (*it).mCentroid = MyMathFunctions::centroidOfTriangle(temp1, temp2, temp3);
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
                if(mTriangles[j].mIndecies[k] == i)
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
