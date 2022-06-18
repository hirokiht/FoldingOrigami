#include "AudioSource.hpp"

AudioSource::AudioSource() {
    alGenSources(1, &source);
}

AudioSource::~AudioSource() {
    alDeleteSources(1, &source);
}
