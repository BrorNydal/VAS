#include "shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <QDebug>

#include "globalconstants.h"

//#include "GL/glew.h" - using QOpenGLFunctions instead

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath, EShaderType shadertype)
    :   mShaderType(shadertype)
{
    initializeOpenGLFunctions();    //must do this to get access to OpenGL functions in QOpenGLFunctions

    qDebug() << "";
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Open files and check for errors
    vShaderFile.open( vertexPath );
    if(!vShaderFile)
        std::cout << "ERROR SHADER FILE " << vertexPath << " NOT SUCCESFULLY READ" << std::endl;
    fShaderFile.open( fragmentPath );
    if(!fShaderFile)
        std::cout << "ERROR SHADER FILE " << fragmentPath << " NOT SUCCESFULLY READ" << std::endl;
    std::stringstream vShaderStream, fShaderStream;
    // Read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf( );
    fShaderStream << fShaderFile.rdbuf( );
    // close file handlers
    vShaderFile.close( );
    fShaderFile.close( );
    // Convert stream into string
    vertexCode = vShaderStream.str( );
    fragmentCode = fShaderStream.str( );

    const GLchar *vShaderCode = vertexCode.c_str( );
    const GLchar *fShaderCode = fragmentCode.c_str( );

    // 2. Compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    // Vertex Shader
    vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex, 1, &vShaderCode, nullptr );
    glCompileShader( vertex );
    // Print compile errors if any
    glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( vertex, 512, nullptr, infoLog );
        std::cout << "ERROR SHADER VERTEX " << vertexPath << " COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    else
    {
        if(FolderPath::PrintInfo)
        {
            qDebug() << vertexPath << " shader was successfully compiled!";
        }
    }
    // Fragment Shader
    fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment, 1, &fShaderCode, nullptr );
    glCompileShader( fragment );
    // Print compile errors if any
    glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( fragment, 512, nullptr, infoLog );
        std::cout << "ERROR SHADER FRAGMENT " << fragmentPath << " COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    else
    {
        if(FolderPath::PrintInfo)
        {
            qDebug() << fragmentPath << " shader was successfully compiled!";
        }
    }
    // Shader Program
    this->ID = glCreateProgram( );
    glAttachShader( this->ID, vertex );
    glAttachShader( this->ID, fragment );
    glLinkProgram( this->ID );
    // Print linking errors if any
    glGetProgramiv( this->ID, GL_LINK_STATUS, &success );
    if (!success)
    {
        glGetProgramInfoLog( this->ID, 512, nullptr, infoLog );
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << "  " << vertexPath <<  "\n   " << infoLog << std::endl;
    }
    else
    {
        if(FolderPath::PrintInfo)
        {
            qDebug() << "Shader linked successfully!";
            qDebug() << "";
        }
    }
    // Delete the shaders as they're linked into our program now and no longer needed
    glDeleteShader( vertex );
    glDeleteShader( fragment );
}

void Shader::use()
{
    glUseProgram( ID );
}

GLuint Shader::getID() const
{
    return ID;
}

void Shader::uniformBool(const std::string &name, bool value)
{
    int loc = glGetUniformLocation(ID, name.c_str());

    if(loc != -1)
        glUniform1i(loc, static_cast<int>(value));
    else
        qDebug() << "Uniform " << name.c_str() << " doesent exist.";
}

void Shader::uniformInt(const std::string &name, int value)
{
    int loc = glGetUniformLocation(ID, name.c_str());

    if(loc != -1)
        glUniform1i(loc, value);
    else
        qDebug() << "Uniform " << name.c_str() << " doesent exist.";
}

void Shader::uniformf(const std::string &name, float value)
{
    int loc = glGetUniformLocation(ID, name.c_str());

    if(loc != -1)
        glUniform1f(loc, value);
    else
        qDebug() << "Uniform " << name.c_str() << " doesent exist.";
}

void Shader::uniform2f(const std::string &name, float v1, float v2)
{
    int loc = glGetUniformLocation(ID, name.c_str());

    if(loc != -1)
        glUniform2f(loc, v1, v2);
    else
        qDebug() << "Uniform " << name.c_str() << " doesent exist.";
}

void Shader::uniform2f(const std::string &name, const vec2 &values)
{
    uniform2f(name, values.x, values.y);
}

void Shader::uniform3f(const std::string &name, float v1, float v2, float v3)
{
    int loc = glGetUniformLocation(ID, name.c_str());

    if(loc != -1)
        glUniform3f(loc, v1, v2, v3);
    else
        qDebug() << "Uniform " << name.c_str() << " doesent exist.";
}

void Shader::uniform3f(const std::string &name, const vec3 &values)
{
    uniform3f(name, values.x, values.y, values.z);
}

void Shader::uniformMat4x4f(const std::string &name, const QMatrix4x4 &mat)
{
    int loc = glGetUniformLocation(ID, name.c_str());

    if(loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, mat.data());
    else
        qDebug() << "Uniform " << name.c_str() << " doesent exist.";
}
