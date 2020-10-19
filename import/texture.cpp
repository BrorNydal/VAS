#include "texture.h"

#include "Vendor/stb_image/stb_image.h"
#include "globalconstants.h"

#include <fstream>
#include <qdebug.h>



Texture::Texture(const std::string& fileNameAndType, unsigned int target)
    :	mTextureID(0), mFilepath(FolderPath::TextureFileDirectory + fileNameAndType), mData(nullptr), mTarget(target), mChannelCount(0)
{
    initializeOpenGLFunctions();
}


Texture::~Texture()
{
    glDeleteTextures(1, &mTextureID);
}

void Texture::bind(unsigned int slot)
{    
    glActiveTexture(GL_TEXTURE0 +  slot);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setAndReadFile(const std::string &file)
{
    setFile(file);
    readFile();
}

void Texture::setFile(const std::string &file)
{
    mFilepath = FolderPath::TextureFileDirectory + file;
}

void Texture::readFile()
{
    //future note... Do different things based on mTarget enum-value.
    //currently only reads 2D-texture

    glGenTextures((GLsizei)1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    mData = stbi_load(mFilepath.c_str(), &mWidth, &mHeight, &mChannelCount, 0);

    if(mData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mData);
        glGenerateMipmap(GL_TEXTURE_2D);    //Generates different sizes of the texture depending on the distance we see the object from

        if(FolderPath::PrintInfo)
        {
            qDebug() << "\n";
            qDebug() << "Texture " << mFilepath.c_str() << " found!";
            qDebug() << "ID: " << mTextureID << ", Resolution : " << mWidth << "x" << mHeight;
            qDebug() << "\n";
        }
    }
    else
        qDebug() << "[WARNING] Texture" << mFilepath.c_str() << " not found.";

    stbi_image_free(mData);
}

unsigned char Texture::getColorAt(unsigned int x, unsigned int y)
{
    return mData[(y * mWidth) + x];
}
