#pragma once
#include <AL/al.h>
#include <string_view>

class AudioBuffer
{
public:
    AudioBuffer(std::string_view filepath);
    ~AudioBuffer();

    constexpr operator ALuint() const {
        return buffer;
    }

private:
    ALuint buffer = 0;
    int rate = -1;
    int channels = -1;
    int64_t samples = -1;
};
