#include "xyz.h"


XYZ::XYZ()
    :   mLineLength(20.f)
{
    mDrawMode = GL_LINES;
    mDrawArrays = true;

    mVertices.push_back(Vertex(-mLineLength, 0.f, 0.f,           1.f, 0.f, 0.f));   //X
    mVertices.push_back(Vertex(mLineLength, 0.f, 0.f,   1.f, 0.f, 0.f));   //X
    mVertices.push_back(Vertex(0.f, -mLineLength, 0.f,           0.f, 1.f, 0.f)); //Y
    mVertices.push_back(Vertex(0.f, mLineLength, 0.f,   0.f, 1.f, 0.f)); //Y
    mVertices.push_back(Vertex(0.f, 0.f, -mLineLength / 2.f,           0.f, 0.f, 1.f)); //Z
    mVertices.push_back(Vertex(0.f, 0.f, mLineLength / 2.f,   0.f, 0.f, 1.f)); //Z
}

XYZ::~XYZ()
{

}
