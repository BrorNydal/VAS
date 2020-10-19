#include "vertex.h"
#include "vec2.h"
#include "vec3.h"

Vertex::Vertex()
{

}

Vertex::Vertex(float X, float Y, float Z, float R, float G, float B, float U, float V)
    :   x(X), y(Y), z(Z), r(R), g(G), b(B), u(U), v(V)
{

}

void Vertex::setXYZ(const vec3 &xyz)  {x = xyz.x; y = xyz.y; z = xyz.z;}
void Vertex::setRGB(const vec3 &rgb)  {r = rgb.x; g = rgb.y; b = rgb.z;}
void Vertex::setUV(const vec2 &uv)    {u = uv.x;  v =  uv.y;}

void Vertex::calculateVertexNormal()
{
    vec3 vertexNormal = vec3::averageVector(mSharedNormals);
    vertexNormal.normalize();
    setRGB(vertexNormal.x, vertexNormal.y, vertexNormal.z);
    mSharedNormals.clear();
    mSharedNormals.shrink_to_fit();
}
