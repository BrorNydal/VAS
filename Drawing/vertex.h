#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <vector>

class vec2;
class vec3;

class Vertex
{
public:
    Vertex();
    Vertex(float X, float Y, float Z,
           float R = 0, float G = 0, float B = 0,
           float U = 0, float V = 0);

    //Positions
    float x, y, z;

    //Normals
    float r, g, b;

    //UV
    float u, v;

    void setXYZ(float X, float Y, float Z)  {x = X; y = Y; z = Z;}
    void setRGB(float R, float G, float B)  {r = R; g = G; b = B;}
    void setUV (float U, float V)           {u = U; v = V;}
    void setXYZ(const class vec3 &xyz);
    void setRGB(const class vec3 &rgb);
    void setUV (const class vec2 &uv);

    friend std::ostream& operator << (std::ostream& os, Vertex &v);
    friend std::istream& operator >> (std::istream& is, Vertex &v);

    std::vector<vec3> mSharedNormals;
    void calculateVertexNormal();
};

inline std::ostream& operator << (std::ostream& os, Vertex &v) {
    //os << std::fixed;
    os << "" << v.x << " " << v.y << " " << v.z << "    ";
    os << "" << v.r << " " << v.g << " " << v.b << " ";
    return os;
}

inline std::istream& operator >> (std::istream& is, Vertex &v) {
    // Trenger fire temporære variabler som kun skal lese inn parenteser og komma
      char dum, dum2, dum3, dum4;
      is >> dum >> v.x >> dum2 >> v.y >> dum3 >> v.z >> dum4;
      is >> dum >> v.r >> dum2 >> v.g >> dum3 >> v.b >> dum4;
      return is;
}
#endif // VERTEX_H
