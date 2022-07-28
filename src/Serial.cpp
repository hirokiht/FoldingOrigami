#include "Serial.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/signal.h>

#include <iostream>
#include <system_error>

Serial::Serial(const char * tty, unsigned baudrate) {
    const auto rc = access(tty, R_OK);
    if (rc)
    {
        throw std::system_error(rc, std::system_category());
    }
	fd =  open(tty,O_RDONLY | O_NOCTTY | O_NDELAY);
	if(fd < 0)
    {
        throw std::system_error(fd, std::system_category());
    }
    /* send SIGIO on io available
	fcntl(fd, F_SETOWN, getpid());
	fcntl(fd, F_SETFL, FASYNC);
    */
    struct termios config;
    if(tcgetattr(fd, &config) < 0)
		throw std::runtime_error("Unable to get attribute");
    config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
    config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
    config.c_cflag &= ~(CSIZE | PARENB);
	config.c_cflag |= CS8;
	cfsetspeed(&config, baudrate);
	config.c_cc[VMIN]  = 1;
	config.c_cc[VTIME] = 0;
	if(tcsetattr(fd, TCSAFLUSH, &config) < 0)
		throw std::runtime_error("Unable to set attribute");
}

char Serial::read() {
    char c = '\0';
    const auto rc = ::read(fd, &c, 1);
    if (rc == 0)
        throw std::runtime_error("Disconnected");
    return c;
}
