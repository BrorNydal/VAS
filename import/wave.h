#ifndef WAVE_H
#define WAVE_H

#ifdef _WIN32
#include <al.h>
#include <alc.h>
#endif
#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#include <iostream>
#include "vec3.h"

class Wave
{
public:
    /// Constructor.
    /**
        Takes two arguments, initialises some variables.
        \param The name of the sound source. Not used.
        \param A boolean to check if sound should loop.
    **/
    Wave(std::string name, bool loop = false, float gain = 1.0);
    ~Wave();

    /// Plays the sound source from the current position.
    void play();
    /// Pauses the sound source at the current position.
    void pause();
    /// Stops the sound source and rewinds to start.
    void stop();

    /// Loads one given WAVE file.
    /**
        Calls the wave loader from the FileHandler class, parses the wave data and buffers it.
        \param The file path relative to execution directory.
    **/
    bool loadWave(std::string filePath);

    void setPosition(vec3 newPos);             ///< Sets source position from Vector3.
    vec3 getPosition() {return mPosition;}     ///< Returns source position as Vector3.
    void setVelocity(vec3 newVel);             ///< Sets source velocity from Vector3.
    vec3 getVelocity() {return mVelocity;}     ///< Returns source velocity as Vector3.

private:
    std::string mName;          ///< The name of the sound source (Not used).
    ALuint mSource;             ///< The sound source.
    ALuint mBuffer;             ///< The data buffer.
    vec3 mPosition;    ///< Vector containing source position.
    vec3 mVelocity;    ///< Vector containing source velocity.
    bool checkError(std::string name);
};

#endif // WAVE_H
