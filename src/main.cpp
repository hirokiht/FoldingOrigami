#include "AudioContext.hpp"
#include "AudioBuffer.hpp"
#include "AudioSource.hpp"
#include "Serial.hpp"
#include <thread>
#include <iostream>
#include <string_view>

#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]){
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <sound file> <serial device> <initial degree>" << std::endl;
        return 1;
    }
    float deg = strtoul(argv[3], NULL, 10) * 3.14 / 180.0;
    Serial tty0{argv[2], 2000000u};
    AudioContext ctx;
    std::cout << "Opened device " << ctx.getDeviceName() << std::endl;

    AudioBuffer se_buffer{argv[1]};
    AudioSource se_source;
    se_source.setBuffer(se_buffer);
    se_source.setLoop(true);
    se_source.setMaxDistance(10);
    float gain = 0;
    se_source.setPosition(deg);
    se_source.setGain(gain);
    se_source.play();
    for (char prev = '0', state ; alGetError() == AL_NO_ERROR ; prev = state)
    {
        std::this_thread::sleep_for(10ms);
	state = tty0.read();
	if (state == '\0')
		continue;
	switch (state)
	{
		case '0':
		    gain = std::max(0., gain - 0.05);
			break;
		case '1':
			gain = std::min(1., gain + 0.50);
			deg -= 0.01;
			break;
		case '2':
			gain = std::min(1., gain + 0.50);
			break;
		case '3':
			gain = std::min(1., gain + 0.50);
			deg += 0.01;
			break;
		default:
			break;
	}
	std::cout << "state " << state << " gain " << gain << " deg " << deg << std::endl;
	if (state != '\0' && prev != state)
		se_source.setGain(state == '0'? 0 : 1);
        if (deg > M_PI)
            deg -= 2 * M_PI;
        se_source.setPosition(deg);
	se_source.setGain(gain);
    }
    return 0;
}
