#include "xyz.h"


XYZ::XYZ()
    :   mLineLength(9.f)
{
    mVertices.push_back(Vertex(0.f, 0.f, 0.f,           1.f, 0.f, 0.f));   //X
    mVertices.push_back(Vertex(mLineLength, 0.f, 0.f,   1.f, 0.f, 0.f));   //X
    mVertices.push_back(Vertex(0.f, 0.f, 0.f,           0.f, 1.f, 0.f)); //Y
    mVertices.push_back(Vertex(0.f, mLineLength, 0.f,   0.f, 1.f, 0.f)); //Y
    mVertices.push_back(Vertex(0.f, 0.f, 0.f,           0.f, 0.f, 1.f)); //Z
    mVertices.push_back(Vertex(0.f, 0.f, 5.f,   0.f, 0.f, 1.f)); //Z
}

XYZ::~XYZ()
{

}

void XYZ::init()
{
    mMatrix.setToIdentity();

   initializeOpenGLFunctions();

   glGenVertexArrays(1, &mVAO);

   //what object to draw
   glBindVertexArray(mVAO);

   glGenBuffers(1, &mVBO);
   glBindBuffer(GL_ARRAY_BUFFER, mVBO);

   //Vertex Buffer Object to hold vertices - VBO
   glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

   glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)(3 * sizeof(GLfloat)));
   glEnableVertexAttribArray(1);
}

void XYZ::draw()
{
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.data());
    glBindVertexArray( mVAO );
    glDrawArrays(GL_LINES, 0, static_cast<int>(mVertices.size()));
}
