#include "uart.hpp"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <iostream>
#include <termios.h>

int main() {
    UART uart("/dev/serial0", B115200);

    // TEMP DEBUGGING
    while (true) {
        std::string line;
        if (uart.readLine(line)) {
            std::cout << "Received: " << line << std::endl;
        }
    }

    return 0;
}
