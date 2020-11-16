#include "vertex.h"

Vertex::Vertex()
{

}

Vertex::Vertex(float X, float Y, float Z, float R, float G, float B, float U, float V)
    :   x(X), y(Y), z(Z), r(R), g(G), b(B), u(U), v(V)
{

}

Vertex::operator QVector3D()
{
    return QVector3D(x, y, z);
}
