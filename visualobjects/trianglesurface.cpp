#include "VisualObjects/trianglesurface.h"

#include "MyMath/mymathfunctions.h"
#include "VisualObjects/octahedronball.h"

#include <fstream>


TriangleSurface::TriangleSurface(std::string fileName)
    :   mSimulationObject(nullptr), mDelta(0), mSpeed(0.012f), mLerpIndex(0), mTriangleIndex(0)
{
    readFile(fileName);
    writeFile();
    findTriangleAdjacency();
    barycentricSearchPath(3.5, 3.5);

    //Use Phong Shader
    mShaderIndex = 1;
}

void TriangleSurface::init()
{
    mMatrix.setToIdentity();

   initializeOpenGLFunctions();

    for(unsigned int i = 0; i < mBarycentricSearchTrace.size(); i++)
         mLineDraw.addPosition(Vector3D(mBarycentricSearchTrace[i].x(), mBarycentricSearchTrace[i].y(), 2.f));

    mLineDraw.setTransformMatrixUniform(mMatrixUniform);
    mLineDraw.init();

    glGenVertexArrays(1, &mVAO);

    //what object to draw
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, mVertices.size() * sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

}

void TriangleSurface::draw()
{
    mLineDraw.draw();


    if(mSimulationObject != nullptr)
    {
        mLineDraw.lerpBetweenPositions(mSimulationObject);
        //lerpObject(mSimulationObject);
        findHeight();
    }

    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.data());
    glBindVertexArray( mVAO );
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(mVertices.size()));
}

void TriangleSurface::readFile(std::string fileName)
{
    std::ifstream file("../GSOpenGL2020/DataSett/" + fileName + ".txt");

    if (file.is_open())
    {
        //If we successfully open the new file, we clear the existing vertices
        mVertices.clear();

        const unsigned int vertexesPerSurface = 3;  //Equals the amount of vertexes creating one surface. Triangles = 3 . . .
        const unsigned int valuesPerVertex = 3;     //equals the amount of values we recieve per vertex from the textfile

        unsigned int count = 0; //while-loop count

        while(!file.eof())
        {
            float fv[vertexesPerSurface][valuesPerVertex];
            OldTriangle newSurface(count); //Creating new triangle with id equal to loop-count ensuring a unique id
            std::vector<Vertex> vs;

            //Reads from file

            for(unsigned int i = 0; i < vertexesPerSurface; i++)
            {
                Vertex v;

                for(unsigned int j = 0; j < valuesPerVertex; j++)
                    file >> fv[i][j];

                v.setXYZ(fv[i][0], fv[i][1], fv[i][2]);
                vs.push_back(v);
            }

            //Create temporary vector3d's from vertices

            Vector3D temp1 = Vector3D(vs[0].x, vs[0].y, vs[0].z);
            Vector3D temp2 = Vector3D(vs[1].x, vs[1].y, vs[1].z);
            Vector3D temp3 = Vector3D(vs[2].x, vs[2].y, vs[2].z);

            Vector3D surfaceNormal;

            //Calculate the relative surface normal using temporary vectors
            //In all cases with all triangles pointing positive in one direction, we can measure the respective axis-value recieved
            if(temp1.calculateNormal(vs[1], vs[2]).z() > 0) //All surfaces are pointing in positive z-direction
                surfaceNormal = temp1.calculateNormal(vs[1], vs[2]);
            else
                surfaceNormal = temp1.calculateNormal(vs[2], vs[1]);


            //Adding surface normal result to vertecies and triangle surface

            newSurface.setFaceNormal(Vector3D(surfaceNormal));

            for(unsigned int i = 0; i < vertexesPerSurface; i++)
                vs[i].setRGB(surfaceNormal.x(), surfaceNormal.y(), surfaceNormal.z());


            //Adding vertecies to vertex-list, and vertex-positions to triangle

            for(unsigned int i = 0; i < vs.size(); i++)
            {
                mVertices.push_back(vs[i]);
                newSurface.mPositions.push_back(Vector3D(vs[i].x, vs[i].y, vs[i].z));
            }

            newSurface.setCentroid(MyMathFunctions::centroidOfTriangle(&newSurface));

            mTriangles.push_back(newSurface);

            count += 1;
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

void TriangleSurface::writeFile()
{
    std::ofstream file("../GSOpenGL2020/DataSett/Oppg_5_2_10.txt");

    if(file.is_open())
    {
        //Creates new file or overrides an existing file with same path
        for(unsigned int i = 0; i < mVertices.size(); i++)
        {
            file << mVertices[i] << "\n";
        }
    }
    else
    {
        QString errormsg = QString("[ERROR] Cannot create and open file.");
        qDebug() << errormsg;
    }

    file.close();
}

//Function needed for function "triangleAdjacency


void TriangleSurface::findTriangleAdjacency()
{
    for(unsigned int i = 0; i < mTriangles.size(); i++)
    {
        OldTriangle *triangle1 = &mTriangles[i];
        Vector3D vec1[3] = {triangle1->mPositions[0], triangle1->mPositions[1], triangle1->mPositions[2]};

        for(unsigned int j = 0; j < mTriangles.size(); j++)
        {
            OldTriangle *triangle2 = &mTriangles[j];

            //if(t1.ID != t2.ID && triangle2 is not in adjacency list of triangle1 && we have not reached maximum adjacent triangles)
            if(triangle1 != triangle2 && !triangle1->isTriangleListed(triangle2) && triangle1->mAdjentTriangles.size() < 3)
            {
                Vector3D vec2[3] = {triangle2->mPositions[0], triangle2->mPositions[1], triangle2->mPositions[2]};

                unsigned int positionsEqual = 0;

                for(unsigned int k = 0; k < 3; k++)
                {
                    if(vec1[k] == vec2[0])
                    {
                        positionsEqual+=1;
                    }
                    if(vec1[k] == vec2[1])
                    {
                        positionsEqual+=1;
                    }
                    if(vec1[k] == vec2[2])
                    {
                        positionsEqual+=1;
                    }
                }

                //If the triangle has 2 equal vertex-positions: it is adjacent
                if(positionsEqual == 2)
                {
                    triangle1->mAdjentTriangles.push_back(triangle2);
                    triangle2->mAdjentTriangles.push_back(triangle1);
                }
            }
        }
    }
}

void TriangleSurface::barycentricSearchPath(float x, float y)
{
    bool found = false;
    std::vector<Vector3D> visitedLocations;

    unsigned int count = 0;

    while(!found)
    {
        OldTriangle *triangle = &mTriangles[count];

        Vector3D bc = Vector2D(x, y) .barycentricCoordinates(   Vector2D(triangle->mPositions[0].x(), triangle->mPositions[0].y()   ),
                                                    Vector2D(triangle->mPositions[1].x(), triangle->mPositions[1].y()   ),
                                                    Vector2D(triangle->mPositions[2].x(), triangle->mPositions[2].y()   ));

        mBarycentricSearchTrace.push_back(triangle->getCentroid());

        //qDebug() << "ID :" << triangle->getID();
        //qDebug() << "BC :" << bc.x() << "," << bc.y() << "," << bc.z();
        if(bc.x() >= 0 && bc.y() >= 0 && bc.z() >= 0)
            found = true;
        else
            count++;

        if(count == mTriangles.size())
            break;

    }


}

void TriangleSurface::barycentricSearchPath(Vector2D loc)
{
    barycentricSearchPath(loc.x(), loc.y());
}

void TriangleSurface::lerpObject(VisualObject *object)
{
    Vector3D result;

    if(mLerpIndex + 1 < mBarycentricSearchTrace.size())
        result = mBarycentricSearchTrace[mLerpIndex + 1] * mDelta + mBarycentricSearchTrace[mLerpIndex] * (1 - mDelta);

    object->setPosition(result);
    mDelta += mSpeed / (mBarycentricSearchTrace[mLerpIndex + 1] - mBarycentricSearchTrace[mLerpIndex]).length();
    if(mDelta >= 1)
    {
        mDelta = 0;

        mLerpIndex++;

        if(mLerpIndex == mBarycentricSearchTrace.size() - 1)
        {
            mLerpIndex = 0;
            mTriangleIndex = 0;
        }
    }
}

void TriangleSurface::findHeight()
{
    Vector2D t1 = Vector2D(mTriangles[mTriangleIndex].mPositions[0].x(), mTriangles[mTriangleIndex].mPositions[0].y());
    Vector2D t2 = Vector2D(mTriangles[mTriangleIndex].mPositions[1].x(), mTriangles[mTriangleIndex].mPositions[1].y());
    Vector2D t3 = Vector2D(mTriangles[mTriangleIndex].mPositions[2].x(), mTriangles[mTriangleIndex].mPositions[2].y());

    Vector3D bc = Vector2D(mSimulationObject->getPosition().x(), mSimulationObject->getPosition().y()).barycentricCoordinates(t1, t2, t3);

    if(bc.x() >= 0 && bc.y() >= 0 && bc.z() >= 0)
        mSimulationObject->setPosition(Vector3D(mSimulationObject->getPosition().x(),
                                mSimulationObject->getPosition().y(),
                                mTriangles[mTriangleIndex].mPositions[0].z() * bc.x() +
                                (mTriangles[mTriangleIndex].mPositions[1].z() * bc.y()) +
                                (mTriangles[mTriangleIndex].mPositions[2].z() * bc.z()) + 0.2f));
    else
    {
        for(unsigned int i = 0; i < mTriangles[mTriangleIndex].mAdjentTriangles.size(); i++)
        {
            if(findNextTriangle(mTriangles[mTriangleIndex].mAdjentTriangles[i]->getID()) == true)
            {
                mTriangleIndex = mTriangles[mTriangleIndex].mAdjentTriangles[i]->getID();
                break;
            }
        }
    }
}

bool TriangleSurface::findNextTriangle(unsigned int index)
{
    Vector2D t1 = Vector2D(mTriangles[index].mPositions[0].x(), mTriangles[index].mPositions[0].y());
    Vector2D t2 = Vector2D(mTriangles[index].mPositions[1].x(), mTriangles[index].mPositions[1].y());
    Vector2D t3 = Vector2D(mTriangles[index].mPositions[2].x(), mTriangles[index].mPositions[2].y());

    Vector3D bc = Vector2D(mSimulationObject->getPosition().x(), mSimulationObject->getPosition().y()).barycentricCoordinates(t1, t2, t3);

    if(bc.x() >= 0 && bc.y() >= 0 && bc.z() >= 0)
        return true;
    else
        return false;
}



