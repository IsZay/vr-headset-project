#include <iostream>
#include <chrono>
#include <thread>
#include "uart.hpp"
#include "cube_renderer.hpp"

int main() {
    // Initialize UART
    UART uart("/dev/serial0", B115200); // I need to add the B115200 baud rate

    // Create renderer
    CubeRenderer renderer; //
    if (!renderer.initialize()) {
        std::cerr << "Failed to initialize renderer." << std::endl;
        return 1;
    }

    while (true) {
        std::string line = uart.readLine();
        float roll, pitch, yaw;

        if (sscanf(line.c_str(), "Roll:%f Pitch:%f Yaw:%f", &roll, &pitch, &yaw) == 3) {
            renderer.setOrientation(roll, pitch, yaw);
        }

        renderer.renderFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));  // ~60 FPS
    }

    return 0;
}


// Before 07/08/25
// #include "uart.hpp"
// #include <EGL/egl.h>
// #include <GLES2/gl2.h>
// #include <iostream>
// #include <termios.h>

// int main() {
//     UART uart("/dev/serial0", B115200);

//     // TEMP DEBUGGING
//     while (true) {
//         std::string line;
//         if (uart.readLine(line)) {
//             std::cout << "Received: " << line << std::endl;
//         }
//     }

//     return 0;
// }
