#include "soundmanager.h"

#include "globalconstants.h"

#include "wavefilehandler.h"
#include <iostream>

SoundManager* SoundManager::mInstance = NULL;

SoundManager::SoundManager() :
    mDevice(NULL),
    mContext(NULL)
{

}

bool SoundManager::init_impl()
{
    if(mInitialized == false)
    {
        std::cout << "Intializing OpenAL!\n";
        mDevice = alcOpenDevice(NULL);
        if (mDevice)
        {
            mContext = alcCreateContext(mDevice, NULL);
            alcMakeContextCurrent(mContext);
        }

        // Generate buffers
        alGetError();

        if (!mDevice)
        {
            std::cout << "Device not made!\n";
        }
        else
            std::cout << "Intialization complete!\n";

        //Start listing of found sound devices:
        //Not jet implemented
        //ALDeviceList *pDeviceList = NULL;
        //ALCcontext *pContext = NULL;
        //ALCdevice *pDevice = NULL;
        //ALint i;	//will hold the number of the preferred device
        //ALboolean bReturn = AL_FALSE;

        mInitialized = true;
        return mInitialized;
    }

    return mInitialized;
}

Wave *SoundManager::createSource_impl(std::string name, vec3 pos, std::string filePath, bool loop, float gain)
{
    Wave* tempPtr = new Wave(name, loop, gain);
    tempPtr->setPosition(pos);
    if (filePath != "")
        tempPtr->loadWave(FolderPath::WaveFileDirectory + filePath + ".wav");

    return tempPtr;
}

SoundManager *SoundManager::get()          ///< Get pointer to singleton instance.
{
    if (!mInstance)
        mInstance = new SoundManager();
    return mInstance;
}

bool SoundManager::init()
{
    get()->init_impl();
}

void SoundManager::cleanUp()
{
    mContext = alcGetCurrentContext();
    mDevice = alcGetContextsDevice(mContext);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(mContext);
    alcCloseDevice(mDevice);
}

bool SoundManager::checkError()
{
    switch (alGetError())
    {
    case AL_NO_ERROR:
        break;
    case AL_INVALID_NAME:
        std::cout << "Invalid name!\n";
        return false;
    case AL_INVALID_ENUM:
        std::cout << "Invalid enum!\n";
        return false;
    case AL_INVALID_VALUE:
        std::cout << "Invalid value!\n";
        return false;
    case AL_INVALID_OPERATION:
        std::cout << "Invalid operation!\n";
        return false;
    case AL_OUT_OF_MEMORY:
        std::cout << "Out of memory!\n";
        return false;
    default: break;
    }
    return true;
}

Wave* SoundManager::createSource(std::string name, vec3 pos, std::string filePath, bool loop, float gain)
{
    return get()->createSource_impl(name, pos, filePath, loop, gain);
}

void SoundManager::updateListener(QVector3D pos, QVector3D vel, QVector3D dir, QVector3D up)
{
    ALfloat posVec[3];
    ALfloat velVec[3];
    ALfloat headVec[6];
    posVec[0] = pos.x();
    posVec[1] = pos.y();
    posVec[2] = pos.z();
    velVec[0] = vel.x();
    velVec[1] = vel.y();
    velVec[2] = vel.z();
    headVec[0] = dir.x();
    headVec[1] = dir.y();
    headVec[2] = dir.z();
    headVec[3] = up.x();
    headVec[4] = up.y();
    headVec[5] = up.z();
    alListenerfv(AL_POSITION, posVec);
    alListenerfv(AL_VELOCITY, velVec);
    alListenerfv(AL_ORIENTATION, headVec);
}
