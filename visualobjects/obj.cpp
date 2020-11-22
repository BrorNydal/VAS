#include "obj.h"

#include "globalconstants.h"
#include <fstream>

OBJ::OBJ(const std::string &file)
    :   mFile(file)
{
    read();
}

bool OBJ::read()
{
    bool success = false;
    std::ifstream file;

    file.open(Path::ObjFileDirectory + mFile);

    if(file.is_open())
    {
        success = true;
        mVertices.clear();

        std::vector<QVector3D> vertexNormalsTemp;
        std::vector<QVector2D> textureCoordinatesTemp;

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

                 vertexNormalsTemp.push_back(QVector3D(vn[0], vn[1], vn[2]));
             }
             else if(indicator == "vt") //Fetch textur coordinate
             {
                 const unsigned int n = 2;
                 float vt[n];

                 for(unsigned int i = 0; i < n; i++)
                     file >> vt[i];

                 textureCoordinatesTemp.push_back(QVector2D(vt[0], vt[1]));
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
                             file >> dum;
                         }
                     }
                 }

                 for(unsigned int i = 0; i < n; i++)
                 {
                    Vertex &vertex = mVertices[f[i][0] - 1];
                    vertex.setUV(textureCoordinatesTemp[f[i][1] - 1]);
                    vertex.setRGB(vertexNormalsTemp[f[i][2] - 1]);

                    mIndices.push_back(f[i][0] - 1);
                 }
             }
        }

        file.close();
    }
    else
    {
        std::cout << "Can't open obj file : " << mFile << std::endl;
    }

    return success;
}

void OBJ::draw(Shader &shader)
{
    VisualObject::draw(shader);
}

