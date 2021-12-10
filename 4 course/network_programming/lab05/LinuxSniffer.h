#ifndef LINUXSNIFFER_H
#define LINUXSNIFFER_H

#include <string>

class LinuxSniffer
{
public:
    LinuxSniffer();
    void start();

private:
    int sock = 0;

    void throwError(const std::string &msg);
};

#endif // LINUXSNIFFER_H
