#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <QVector3D>

class Vertex
{
public:
    Vertex();
    Vertex(float X, float Y, float Z,
           float R = 0.f, float G = 0.f, float B = 0.f,
           float U = 0.f, float V = 0.f);

    //Positions
    float x, y, z;

    //Normals
    float r, g, b;

    //texcoords
    float u, v;

    void setXYZ(float X, float Y, float Z){x = X; y = Y; z = Z;}
    void setRGB(float R, float G, float B){r = R; g = G; b = B;}

    operator QVector3D ();

    bool operator < (const Vertex &v);


    friend std::ostream& operator << (std::ostream& os, Vertex &v);
    friend std::istream& operator >> (std::istream& is, Vertex &v);
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
