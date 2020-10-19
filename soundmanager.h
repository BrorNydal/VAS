#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#ifdef _WIN32
#include <al.h>
#include <alc.h>
#endif
#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#include <string>
#include <vector>
#include "wave.h"
#include "vec3.h"
class SoundManager
{
public:
    struct SoundListener
    {
        vec3 pos;
        vec3 vel;
        vec3 dir;
        vec3 up;
        SoundListener()
        {
            pos = vec3(0.0f, 0.0f, 0.0f);
            vel = vec3(0.0f, 0.0f, 0.0f);
            dir = vec3(0.0f, 0.0f, 1.0f);
            up =  vec3(0.0f, 1.0f, 0.0f);
            //Could be something like this, if Vector3-class had support support:
            //pos = Vector3::ZERO;
            //vel = Vector3::ZERO;
            //dir = Vector3::UNIT_Z;
            //up = Vector3::UNIT_Y;
        }
    };

    static SoundManager* get();

    /// Initialises OpenAL.
    static bool init();
    /// Cleans up and shuts down OpenAl.
    void cleanUp();

    /// Creates a new SoundSource with the given parameters.
    /**
        \param The name of the sound. (Not in use.)
        \param The source position as Vector3.
        \param File path relative to execution directory.
        \param Boolean to see if sound should loop or not.
    **/
    static Wave* createSource(std::string name, vec3 pos, std::string filePath = "", bool loop = false, float gain = 1.0);
    void updateListener(QVector3D pos, QVector3D vel, QVector3D dir, QVector3D up);

private:
    SoundManager();                         ///< Private constructor.
    SoundManager(SoundManager const&);      ///< Private copy constructor.
    void operator=(SoundManager const&);    ///< Private class assignment operator.
    static SoundManager* mInstance;         ///< Singleton instance pointer.

    bool init_impl();
    Wave* createSource_impl(std::string name, vec3 pos, std::string filePath = "", bool loop = false, float gain = 1.0);

    /// Debug function.
    /**
        Checks for and outputs OpenAL errors.
    **/
    bool checkError();

    ALCdevice* mDevice;                 ///< Pointer to the ALC Device.
    ALCcontext* mContext;               ///< Pointer to the ALC Context.

    SoundListener mListener;               ///< The listener data structure. (Temporary)

    bool mInitialized = false;
};

#endif // SOUNDMANAGER_H
