#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions_4_1_Core>

#include <QVector3D>
#include "structs.h"

//#include "GL/glew.h" //We use QOpenGLFunctions instead, so no need for Glew (or GLAD)!

//This class is pretty much a copy of the shader class at
//https://github.com/SonarSystems/Modern-OpenGL-Tutorials/blob/master/%5BLIGHTING%5D/%5B8%5D%20Basic%20Lighting/Shader.h
//which is based on stuff from http://learnopengl.com/ and http://open.gl/.

//must inherit from QOpenGLFunctions_4_1_Core, since we use that instead of glfw/glew/glad
class Shader : protected QOpenGLFunctions_4_1_Core
{
private:
    GLuint ID;
    EShader mType;

public:
    // Constructor generates the shader on the fly
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath, EShader type);

    // use this shader
    void use();

    //Get program number for this shader
    GLuint getID() const;
    EShader getType() const;

    // utility uniform functions
    void uniformBool(const std::string &name, bool value);
    void uniformInt(const std::string &name, int value);
    void uniformf(const std::string &name, float value);
    void uniform2f(const std::string &name, float v1, float v2);
    void uniform2f(const std::string &name, const QVector2D &values);
    void uniform3f(const std::string &name, float v1, float v2, float v3);
    void uniform3f(const std::string &name, const QVector3D &values);
    void uniformMat4x4f(const std::string &name, const QMatrix4x4 &mat);


};

#endif
