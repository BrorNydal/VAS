#include "light.h"

Light::Light()
    :   mIntensity(1.4f), mLightColor(Vector3D(0.5f, 0.5f, 0.5f))
{

}

void Light::init()
{
    mMatrix.setToIdentity();

   initializeOpenGLFunctions();

   mVertices.push_back(Vertex(0.f, 0.f, 0.f, 0.f, 0.f, -1.f));
   mVertices.push_back(Vertex(1.f, 0.f, 0.f, 0.f, 0.f, -1.f));
   mVertices.push_back(Vertex(0.5f, 0.8f, 0.f, 0.f, 0.f, -1.f));

   //Use Phong Shader
   mShaderIndex = 1;

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

void Light::draw()
{
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.data());
    glUniform1f(mLightUniforms.mIntensityUniform, mIntensity);
    glUniform3f(mLightUniforms.mLightPositionUniform, mMatrix(0, 3), mMatrix(1, 3), mMatrix(2, 3));
    glUniform3f(mLightUniforms.mLightColorUniform, mLightColor.x(), mLightColor.y(), mLightColor.z());

    glBindVertexArray( mVAO );
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(mVertices.size()));
}
