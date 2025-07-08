#include "uart.hpp"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

UART::UART(const std::string& device, int baud) {
    fd = open(device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        perror("Unable to open UART");
        exit(1);
    }

    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, baud);
    cfsetospeed(&options, baud);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CRTSCTS;
    tcsetattr(fd, TCSANOW, &options);
}

UART::~UART() {
    close(fd);
}

// Changed this to read continually until a newline is found
bool UART::readLine(std::string& output) {
    static std::string buffer;
    char byte;
    while (read(fd, &byte, 1) == 1) {
        if (byte == '\n') {
            output = buffer;
            buffer.clear();
            return true;
        } else if (byte != '\r') {
            buffer += byte;
        }
    }
    return false;
}
