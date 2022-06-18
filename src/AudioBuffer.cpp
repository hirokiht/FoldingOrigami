#include "AudioBuffer.hpp"

#include <AL/alext.h>
#include <sndfile.hh>

#include <limits>
#include <stdexcept>
#include <vector>

AudioBuffer::AudioBuffer(std::string_view path) {
    static_assert(sizeof(short) == sizeof(int16_t), "Only supports short == int16_t");
    SndfileHandle file(path.data());
    if (!file)
        throw std::runtime_error("failed to load audio file");
    auto frames = file.frames();
    if (frames < 1)
        throw std::runtime_error("Invalid number of frames");
    rate = file.samplerate();
    channels = file.channels();
    if (channels < 1)
        throw std::runtime_error("Invalid number of channels in ogg vorbis file");
    std::vector<short> pcmdata(frames * channels);
    if (pcmdata.size() * sizeof(short) > std::numeric_limits<ALsizei>::max())
        throw std::runtime_error("Audio file is too big to fit in buffer");
    frames = file.readf(pcmdata.data(), frames);
    const auto ambisonic = file.command(SFC_WAVEX_GET_AMBISONIC, NULL, 0);
    ALenum format;
    if (ambisonic == SF_AMBISONIC_B_FORMAT) {
        switch (channels) {
            case 3:
                format = AL_FORMAT_BFORMAT2D_16;
                break;
            case 4:
                format = AL_FORMAT_BFORMAT3D_16;
                break;
            default:
                throw std::runtime_error("Unsupported number of channels for ambisonic audio!");
        }
    } else {
        switch (channels) {
            case 1:
                format = AL_FORMAT_MONO16;
                break;
            case 2:
                format = AL_FORMAT_STEREO16;
                break;
            case 3:
                format = AL_FORMAT_BFORMAT2D_16;
                break;
            case 4:
                format = AL_FORMAT_QUAD16;
                break;
            case 6: // 5.1 channels
                format = AL_FORMAT_51CHN16;
                break;
            case 7: // 6.1 channels
                format = AL_FORMAT_61CHN16;
                break;
            case 8: // 7.1 channels
                format = AL_FORMAT_71CHN16;
                break;
            default:
                throw std::runtime_error("Unsupported number of channels!");
        }
    }
    alGenBuffers(1, &buffer);
    const auto bytes = frames * channels * sizeof(short);
    alBufferData(buffer, format, pcmdata.data(), bytes, rate);
    auto error = alGetError();
    if (error != AL_NO_ERROR)
        throw std::runtime_error("Failed to fill in audio buffer");
}

AudioBuffer::~AudioBuffer()
{
    alDeleteBuffers(1, &buffer);
}
