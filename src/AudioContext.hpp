#pragma once
#include "AL/alc.h"

class AudioContext {
public:
    AudioContext();
    AudioContext(ALCdevice *dev);
    ~AudioContext();

    const char * getDeviceName() const {
        const auto name = alcGetString(dev,
                alcIsExtensionPresent(dev, "ALC_ENUMERATE_ALL_EXT")?
                    ALC_ALL_DEVICES_SPECIFIER : ALC_DEVICE_SPECIFIER);
        return name;
    }

private:
    ALCdevice *dev = nullptr;
    ALCcontext *ctx = nullptr;
};
