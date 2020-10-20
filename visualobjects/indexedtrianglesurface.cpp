#include "indexedtrianglesurface.h"

#include "MyMath/mymathfunctions.h"

#include <fstream>

IndexedTriangleSurface::IndexedTriangleSurface(std::string data, std::string index)
    :   mObjectTriangleIndex(0)
{
    readDataFile(data);
    readIndexFile(index);
    calculateSurfaceNormal();
    calculateVertexNormal();
    writeFile();
    barycentricSearchPath(3.5, 3.5);

    //Use Phong Shader
    mShaderIndex = 1;
}

void IndexedTriangleSurface::init()
{
    mMatrix.setToIdentity();

    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mEAB);
    glGenBuffers(1, &mVBO);

    //what object to draw
    glBindVertexArray(mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<unsigned int>(mIndices.size()) * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

}

void IndexedTriangleSurface::draw()
{
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.data());
    glBindVertexArray( mVAO );
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, NULL);
}

void IndexedTriangleSurface::printDebugInformation()
{
    qDebug() << "TRIANGLE CENTROIDS:";
    for(unsigned int i = 0; i < mTriangles.size(); i++)
        qDebug() << "Triangle" << i << ":" << mTriangles[i].mCentroid.x() << mTriangles[i].mCentroid.y() << mTriangles[i].mCentroid.z();

}

void IndexedTriangleSurface::readDataFile(std::string fileName)
{
    std::ifstream file("../GSOpenGL2020/DataSett/" + fileName + ".txt");

    if (file.is_open())
    {
        //If we successfully open the new file, we clear the existing vertices
        mVertices.clear();

        unsigned int count = 0; //while-loop count

        while(!file.eof())
        {
            float v1, v2, v3;
            file >> v1 >> v2 >> v3;

            mVertices.push_back(Vertex(v1, v2, v3));
            count++;
        }

        file.close();
    }
}

void IndexedTriangleSurface::readIndexFile(std::string fileName)
{
    std::ifstream file("../GSOpenGL2020/DataSett/" + fileName + ".txt");

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
            t.mIndecies.push_back(v1);
            t.mIndecies.push_back(v2);
            t.mIndecies.push_back(v3);

            unsigned int i1, i2, i3;
            file >> i1 >> i2 >> i3;

            t.mAdjacentTriangles.push_back(i1);
            t.mAdjacentTriangles.push_back(i2);
            t.mAdjacentTriangles.push_back(i3);

            mTriangles.push_back(t);

            count++;
        }

        file.close();
    }
    else{
        QString errormsg = QString("[ERROR] Cannot open file from path: ") +
                QString::fromStdString(std::string("../GSOpenGL2020/DataSett/") + fileName + ".txt") +
                " , " + typeid (this).name();
        qDebug() << errormsg;
    }
}

void IndexedTriangleSurface::writeFile()
{
    std::string vertexFileName = "Oppg_5_2_11_VertexData";
    std::ofstream vertexFile("../GSOpenGL2020/DataSett/" + vertexFileName + ".txt");

    if(vertexFile.is_open())
    {
        //Creates new file or overrides an existing file with same path
        for(unsigned int i = 0; i < mVertices.size(); i++)
        {
            vertexFile << mVertices[i] << "\n";
        }
    }else{
        QString errormsg = QString("[ERROR] Cannot create file to path: ") +
                QString::fromStdString(std::string("../GSOpenGL2020/DataSett/" + vertexFileName)) +
                " , " + typeid (this).name();
        qDebug() << errormsg;
    }

    vertexFile.close();

    std::string indexFileName = "Oppg_5_2_11_IndexData";
    std::ofstream indexFile("../GSOpenGL2020/DataSett/" + indexFileName + ".txt");

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
        QString errormsg = QString("[ERROR] Cannot create file to path: ") +
                    QString::fromStdString(std::string("../GSOpenGL2020/DataSett/" + indexFileName)) +
                    " , " + typeid (this).name();
        qDebug() << errormsg;
    }

    vertexFile.close();
}

void IndexedTriangleSurface::barycentricSearchPath(float x, float y)
{
    bool found = false;

    while (!found) {
        Vector3D t[3];
        for(unsigned int i = 0; i < 3; i++)
            t[i] = Vector3D(mVertices[mTriangles[mTriangleIndex].mIndecies[i]].x,
                    mVertices[mTriangles[mTriangleIndex].mIndecies[i]].y,
                    mVertices[mTriangles[mTriangleIndex].mIndecies[i]].z);

        Vector3D bc = Vector2D(x, y).barycentricCoordinates(Vector2D(t[0].x(), t[1].y()), Vector2D(t[1].x(), t[1].y()), Vector2D(t[2].x(), t[2].y()));

        mBarycentricSearchTrace.push_back(Vector3D(mTriangles[mTriangleIndex].mCentroid));
        mBarycentricSearchTriangleTrace.push_back(mTriangles[mTriangleIndex].ID);

        //qDebug() << "Centroid : " << mTriangles[mTriangleIndex].mCentroid.x() << mTriangles[mTriangleIndex].mCentroid.y() << mTriangles[mTriangleIndex].mCentroid.z();

        if(bc.x() >= 0 && bc.y() >= 0 && bc.z() >= 0)
        {
            found = true;
        }
        else
        {
            const unsigned int size = 3;
            float bcf[size] = {bc.x(), bc.y(), bc.z()};
            unsigned int indecies[size] = {0, 1, 2};

            //qDebug() << "Triangle"<< mTriangleIndex << ":";
            //qDebug() << "Unsorted Baryc : " << bcf[0] << bcf[1] << bcf[2];

            MySortFunctions::selectionSort<float>(bcf, indecies, size);


//            for(unsigned int i = 0; i < size; i++)
//            {
//                qDebug() << "Sorted baryc" << i << ":" << bcf[i] << "," << indecies[i];
//            }

            for(unsigned int i = 0; i < size; i++)
            {
                if(mTriangles[mTriangleIndex].mAdjacentTriangles[indecies[i]] != -1)
                {
                    mTriangleIndex = static_cast<unsigned int>(mTriangles[mTriangleIndex].mAdjacentTriangles[indecies[i]]);
                    break;
                }
            }
        }

    }
}

void IndexedTriangleSurface::barycentricSearchPath(Vector2D coordinates)
{
    barycentricSearchPath(coordinates.x(), coordinates.y());
}

void IndexedTriangleSurface::barycentricHeightSearch(VisualObject *object, unsigned int initialTriangle)
{
    Vector3D objPosition = object->getPosition();

    Vector3D t[3];
    for(unsigned int i = 0; i < 3; i++)
        t[i] = Vector3D(mVertices[mTriangles[mObjectTriangleIndex].mIndecies[i]].x,
                        mVertices[mTriangles[mObjectTriangleIndex].mIndecies[i]].y,
                        mVertices[mTriangles[mObjectTriangleIndex].mIndecies[i]].z);

    Vector3D bc = Vector2D(objPosition.x(), objPosition.y()).barycentricCoordinates(Vector2D(t[0].x(), t[0].y()),
                                                                                    Vector2D(t[1].x(), t[1].y()),
                                                                                    Vector2D(t[2].x(), t[2].y()));
    if(bc.x() >= 0 && bc.y() >= 0 && bc.z() >= 0)
    {
        object->setHeight(t[0].z() * bc.x() + t[1].z() * bc.y() + t[2].z() * bc.z() + 0.2f);
    }
    else
    {
        if(mIndex < mBarycentricSearchTriangleTrace.size())
        {
            mObjectTriangleIndex = mBarycentricSearchTriangleTrace[mIndex];
            mIndex ++;
        }
        if(mIndex == mBarycentricSearchTriangleTrace.size())
            mIndex = 0;
    }
}

void IndexedTriangleSurface::calculateSurfaceNormal()
{
    for(auto it = mTriangles.begin(); it != mTriangles.end(); it++)
    {
        Vector3D temp1 = Vector3D(mVertices[(*it).mIndecies[0]].x, mVertices[(*it).mIndecies[0]].y, mVertices[(*it).mIndecies[0]].z);
        Vector3D temp2 = Vector3D(mVertices[(*it).mIndecies[1]].x, mVertices[(*it).mIndecies[1]].y, mVertices[(*it).mIndecies[1]].z);
        Vector3D temp3 = Vector3D(mVertices[(*it).mIndecies[2]].x, mVertices[(*it).mIndecies[2]].y, mVertices[(*it).mIndecies[2]].z);

        Vector3D surfaceNormal;

        //Calculate the relative surface normal using temporary vectors
        //In all cases with all triangles pointing positive in one direction, we can measure the respective axis-value recieved
        if(temp1.calculateNormal(mVertices[(*it).mIndecies[1]], mVertices[(*it).mIndecies[2]]).z() > 0) //All surfaces are pointing in positive z-direction
            surfaceNormal = temp1.calculateNormal(mVertices[(*it).mIndecies[1]], mVertices[(*it).mIndecies[2]]);
        else
            surfaceNormal = temp1.calculateNormal(mVertices[(*it).mIndecies[2]], mVertices[(*it).mIndecies[1]]);

        surfaceNormal.normalize();
        (*it).mSurfaceNormal = surfaceNormal;
         //qDebug() << (*it).surfaceNormal.x() << (*it).surfaceNormal.y() << (*it).surfaceNormal.z();

        //While we already have its position, grab its centroid
        (*it).mCentroid = MyMathFunctions::centroidOfTriangle(temp1, temp2, temp3);
        //qDebug() << (*it).mCentroid.x() << (*it).mCentroid.y() << (*it).mCentroid.z();
    }
}

void IndexedTriangleSurface::calculateVertexNormal()
{
    //For each vertex we calculate average normals of surrounding surfaces
    for(unsigned int i = 0; i < mVertices.size(); i++)
    {
        std::vector<Vector3D> surfaceNormals;

        //For each triangle we look at its indecies
        for(unsigned int j = 0; j < mTriangles.size(); j++)
        {
            //Each triangle index will be between 0 and mVertices.size()
            //And atleast one triangle will have an index equal to i
            for(unsigned int k = 0; k < mTriangles[j].mIndecies.size(); k++)
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
        Vector3D averageNormal = MyMathFunctions::average3DVector(surfaceNormals);
        mVertices[i].setRGB(averageNormal.x(), averageNormal.y(), averageNormal.z());
    }
}
