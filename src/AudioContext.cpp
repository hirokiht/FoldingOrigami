#include "AudioContext.hpp"

#include <stdexcept>

AudioContext::AudioContext(ALCdevice *dev) {
    if (dev == nullptr)
        throw std::runtime_error("Audio device not found");
    ctx = alcCreateContext(dev, nullptr);
    if (ctx == nullptr || alcMakeContextCurrent(ctx) == ALC_FALSE)
        throw std::runtime_error("Failed to create audio context");
}

AudioContext::AudioContext() : AudioContext(dev = alcOpenDevice(nullptr)) {
}

AudioContext::~AudioContext() {
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(ctx);
    alcCloseDevice(dev);
}
