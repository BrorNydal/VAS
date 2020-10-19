#include "vertex.h"
#include "MyMath/vector2d.h"
#include "MyMath/vector3d.h"

Vertex::Vertex()
{

}

Vertex::Vertex(float X, float Y, float Z, float R, float G, float B, float U, float V)
    :   x(X), y(Y), z(Z), r(R), g(G), b(B), u(U), v(V)
{

}

void Vertex::setXYZ(const class Vector3D &xyz)  {x = xyz.x(); y = xyz.y(); z = xyz.z();}
void Vertex::setRGB(const class Vector3D &rgb)  {r = rgb.x(); g = rgb.y(); b = rgb.z();}
void Vertex::setUV(const class Vector2D &uv)    {u = uv.x(); v = uv.y();}
