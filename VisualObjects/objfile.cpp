#include "objfile.h"

#include "GlobalConstants.h"

#include <fstream>

ObjFile::ObjFile(std::string fileName)
{
    initializeOpenGLFunctions();
    readFile(fileName);
}

void ObjFile::readFile(std::string fileName)
{
    std::ifstream file((GlobalConstans::ObjFileDirectory + fileName + ".obj"));

    if(file.is_open())
    {
        //Assert and clear for new vertices...

        mName = fileName;

        mVertices.clear();
        mVertexNormals.clear();
        mTextureCoordinates.clear();

        //Indicates what type of value we are fetching
        //v = vertex, vn = vertex normals, vt = texture coordinates, f = face
        std::string indicator;

        while (!file.eof())
        {
             file >> indicator;

             if(indicator == "v") //Fetch vertex
             {
                 const unsigned int n = 3;
                 float v[n];

                 for(unsigned int i = 0; i < n; i++)
                     file >> v[i];

                 mVertices.push_back(Vertex(v[0], v[1], v[2]));
             }
             else if(indicator == "vn") //Fetch vertex normal
             {
                 const unsigned int n = 3;
                 float vn[n];

                 for(unsigned int i = 0; i < n; i++)
                     file >> vn[i];

                 mVertexNormals.push_back(Vector3D(vn[0], vn[1], vn[2]));
             }
             else if(indicator == "vt") //Fetch textur coordinate
             {
                 const unsigned int n = 2;
                 float vt[n];

                 for(unsigned int i = 0; i < n; i++)
                     file >> vt[i];

                 mTextureCoordinates.push_back(Vector2D(vt[0], vt[1]));
             }
             else if(indicator == "f") //Fetch face
             {
                 // Vertex1 (VertexIndex/UV/Normal)
                 // Vertex2 (VertexIndex/UV/Normal)
                 // Vertex3 (VertexIndex/UV/Normal)

                 const unsigned int n = 3;
                 const unsigned int n2 = 3;
                 unsigned int f[n][n2];

                 for(unsigned int i = 0; i < n; i++)
                 {
                     for(unsigned int j = 0; j < n2; j++)
                     {
                         file >> f[i][j];

                         if(j != n2 - 1)
                         {
                             char dum;
                             //file >> dum;
                         }
                     }
                 }

                 for(unsigned int i = 0; i < n; i++)
                 {
                    Vertex &vertex = mVertices[f[i][0] - 1];
                    vertex.setRGB(mVertexNormals[f[i][2] - 1]);
                    vertex.setUV(mTextureCoordinates[f[i][1] - 1]);

                    mIndices.push_back(f[i][0] - 1);
                 }
             }
        }

        file.close();
    }
    else{
        QString errormsg = QString("[WARNING] Cannot open file from path: ") +
                QString::fromStdString(GlobalConstans::ObjFileDirectory + fileName + ".obj") +
                " , " + typeid (this).name();
        qDebug() << errormsg;
    }
}

void ObjFile::init()
{
    mMatrix.setToIdentity();

    mShaderIndex = 1;

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mEAB);
    glGenBuffers(1, &mVBO);

    //what object to draw
    glBindVertexArray(mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(mVertices.size()*sizeof( Vertex )), mVertices.data(), GL_STATIC_DRAW );

    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof( Vertex ), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ),  (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<unsigned int>(mIndices.size()) * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

}

void ObjFile::draw()
{
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.data());
    glBindVertexArray( mVAO );
    glDrawElements(GL_TRIANGLES, static_cast<int>(mIndices.size()), GL_UNSIGNED_INT, NULL);
}
