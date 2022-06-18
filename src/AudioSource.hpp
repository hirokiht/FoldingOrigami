#pragma once
#include <AL/al.h>
#include <cmath>

class AudioSource
{
public:
    AudioSource();
    ~AudioSource();

    inline ALenum state() const {
        ALenum state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        return state;
    }

    inline ALfloat offset() const {
        ALfloat offset;
        alGetSourcef(source, AL_SEC_OFFSET, &offset);
        return offset;
    }

    inline void setBuffer(ALuint buffer) {
        alSourcei(source, AL_BUFFER, static_cast<ALint>(buffer));
    }

    inline void setLoop(bool loop) {
        alSourcei(source, AL_LOOPING, loop? AL_TRUE : AL_FALSE);
    }

    inline void setRelative(bool relative) {
        alSourcei(source, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);
    }

    inline void setReferenceDistance(float distance) {
        alSourcef(source, AL_REFERENCE_DISTANCE, distance);
    }

    inline void setMaxDistance(float distance) {
        alSourcef(source, AL_MAX_DISTANCE, distance);
    }

    inline void setPosition(float x, float y, float z) {
        alSource3f(source, AL_POSITION, x, y, z);
    }

    inline void setPosition(double deg, double distance = 1.0) {
        const auto xpos = cos(deg) * distance, ypos = sin(deg) * distance;
        setPosition(xpos, ypos, 0);
    }

    inline void setVelocity(float x, float y, float z) {
        alSource3f(source, AL_VELOCITY, x, y, z);
    }

    inline void setGain(float gain) {
        alSourcef(source, AL_GAIN, gain);
    }

    inline void setPitch(float pitch) {
        alSourcef(source, AL_PITCH, pitch);
    }

    inline void play() {
        alSourcePlay(source);
    }

    inline void pause() {
        alSourcePause(source);
    }

private:
    ALuint source = 0;
};
