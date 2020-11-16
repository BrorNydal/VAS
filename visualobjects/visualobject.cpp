#include "visualobject.h"




void VisualObject::init()
{
    initializeBuffers_impl(mIndices.data(), mVertices.data(), mIndices.size(), mVertices.size(), mBuffers, mDrawMode);
}

void VisualObject::draw(Shader &shader)
{
    QMatrix4x4 mat4x4;
    mat4x4.setToIdentity();

    mat4x4.translate(mTransform.location);
    mat4x4.scale(mTransform.scale);
    mat4x4.rotate(mTransform.rotation.x(), QVector3D(1.f,0.f,0.f));
    mat4x4.rotate(mTransform.rotation.y(), QVector3D(0.f,1.f,0.f));
    mat4x4.rotate(mTransform.rotation.z(), QVector3D(0.f,0.f,1.f));

    shader.uniformMat4x4f("matrix", mat4x4);
    glBindVertexArray( mBuffers.vao );

    if(mDrawArrays)
        glDrawArrays(mDrawMode, 0, static_cast<int>(mVertices.size()));
    else
        glDrawElements(mDrawMode, mBuffers.indexCount, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

void VisualObject::initializeBuffers_impl(unsigned int *indexData, Vertex *vertexData, unsigned int indexCount, unsigned int vertexCount, Buffers &buffer, GLenum drawing)
{
    buffer.indexCount = indexCount;
    buffer.vertexCount = vertexCount;

    glGenVertexArrays(1, &buffer.vao);
    glGenBuffers(1, &buffer.eab);
    glGenBuffers(1, &buffer.vbo);

    //what object to draw
    glBindVertexArray(buffer.vao);

    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertexCount*sizeof( Vertex )), vertexData, GL_STATIC_DRAW );

    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof( Vertex ), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ),  (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.eab);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<unsigned int>(indexCount) * sizeof(GLuint), indexData, GL_STATIC_DRAW);

    glBindVertexArray(0);
}
