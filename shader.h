#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions_4_1_Core>

#include "enumclasses.h"

#include "vec2.h"
#include "vec3.h"
#include <QMatrix4x4>

//#include "GL/glew.h" //We use QOpenGLFunctions instead, so no need for Glew (or GLAD)!

//This class is pretty much a copy of the shader class at
//https://github.com/SonarSystems/Modern-OpenGL-Tutorials/blob/master/%5BLIGHTING%5D/%5B8%5D%20Basic%20Lighting/Shader.h
//which is based on stuff from http://learnopengl.com/ and http://open.gl/.

//must inherit from QOpenGLFunctions_4_1_Core, since we use that instead of glfw/glew/glad
class Shader : protected QOpenGLFunctions_4_1_Core
{
   GLuint ID;
   EShaderType mShaderType;

public:
    // Constructor generates the shader on the fly
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath, EShaderType shadertype);

    // Use the current shader
    void use( );

    //Get program number for this shader
    GLuint getID() const;

    // utility uniform functions
    void uniformBool(const std::string &name, bool value);
    void uniformInt(const std::string &name, int value);
    void uniformf(const std::string &name, float value);
    void uniform2f(const std::string &name, float v1, float v2);
    void uniform2f(const std::string &name, const vec2 &values);
    void uniform3f(const std::string &name, float v1, float v2, float v3);
    void uniform3f(const std::string &name, const vec3 &values);
    void uniformMat4x4f(const std::string &name, const QMatrix4x4 &mat);

};

#endif
