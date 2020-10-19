#ifndef TEXTURE_H
#define TEXTURE_H

#include <QOpenGLFunctions_4_1_Core>
#include <iostream>

class Texture : public QOpenGLFunctions_4_1_Core
{
    unsigned int mTextureID;
    std::string mFilepath;
    unsigned char* mData;
    GLenum mTarget;

    int mWidth, mHeight, mChannelCount;

public:
    //Assuming texture is in "Textures" folder, filpath is the path from this folder
    Texture(const std::string& fileNameAndType = "Basic.bmp", unsigned int target = GL_TEXTURE_2D);
    ~Texture();

    unsigned int id() const { return mTextureID; }

    unsigned char *getData() {return mData;}

    void bind(unsigned int slot = 0);
    void unbind();

    int getWidth() const { return mWidth; }
    int getHeight() const { return mHeight; }
    int getChannelCount() const {return mChannelCount;}
    GLenum getTarget() const {return mTarget;}

    void setAndReadFile(const std::string &file);
    void setFile(const std::string &file);
    void readFile();


    unsigned char getColorAt(unsigned int x, unsigned int y);
};

#endif // TEXTURE_H
