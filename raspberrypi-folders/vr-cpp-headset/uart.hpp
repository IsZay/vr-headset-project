#pragma once
#include <string>

class UART {
public:
    UART(const std::string& device, int baud);
    ~UART();
    bool readLine(std::string& output);

private:
    int fd;
};