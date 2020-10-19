#include "obj.h"

#include "globalconstants.h"

#include <fstream>

OBJ::OBJ()
{
}

Vertex* OBJ::getVertexData()
{
    return mVertices.data();
}

unsigned int *OBJ::getIndexData()
{
    return mIndices.data();
}

unsigned int OBJ::vertexCount() const
{
    return mVertices.size();
}

unsigned int OBJ::indexCount() const
{
    return mIndices.size();
}

bool OBJ::operator ==(const OBJ &obj) const
{
    return mTag == obj.mTag;
}

bool OBJ::operator !=(const OBJ &obj) const
{
    return mTag != obj.mTag;
}

std::string OBJ::getTag() const
{
    return mTag;
}

bool OBJ::readOBJ(std::string fileName)
{
    bool success = false;
    std::ifstream file((FolderPath::ObjFileDirectory + fileName));

    if(file.is_open())
    {
        success = true;

        //Assert and clear for new vertices...
        mTag = fileName;
        mVertices.clear();

        std::vector<vec3> vertexNormalsTemp;
        std::vector<vec2> textureCoordinatesTemp;

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

                 vertexNormalsTemp.push_back(vec3(vn[0], vn[1], vn[2]));
             }
             else if(indicator == "vt") //Fetch textur coordinate
             {
                 const unsigned int n = 2;
                 float vt[n];

                 for(unsigned int i = 0; i < n; i++)
                     file >> vt[i];

                 textureCoordinatesTemp.push_back(vec2(vt[0], vt[1]));
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
        std::cout << "Can't open obj file : " << fileName << std::endl;
    }

    return success;
}
