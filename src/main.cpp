#include "AudioContext.hpp"
#include "AudioBuffer.hpp"
#include "AudioSource.hpp"
#include "Serial.hpp"
#include <thread>
#include <iostream>
#include <string_view>

using namespace std;

int main(int argc, char *argv[]){
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <bgm file> <effect file> <tty>" << std::endl;
        return 1;
    }
    Serial tty0{argv[3], 2000000u};
    AudioContext ctx;
    std::cout << "Opened device " << ctx.getDeviceName() << std::endl;
    AudioBuffer bgm_buffer{argv[1]};
    AudioSource bgm_source;
    bgm_source.setBuffer(bgm_buffer);
    bgm_source.play();

    AudioBuffer se_buffer{argv[2]};
    AudioSource se_source;
    se_source.setBuffer(se_buffer);
    se_source.setLoop(true);
    se_source.setMaxDistance(10);
    float deg = 0;
    se_source.setPosition(deg);
    se_source.play();
    while (bgm_source.state() == AL_PLAYING && alGetError() == AL_NO_ERROR)
    {
        std::this_thread::sleep_for(20ms);
        deg += 0.005;
        if (deg > M_PI)
            deg -= 2 * M_PI;
        se_source.setPosition(deg);
        std::cout << deg << std::endl;
        std::cout << "tty0: " << tty0.read() << std::endl;
    }
    return 0;
}
