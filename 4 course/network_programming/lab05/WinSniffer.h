#ifndef WINSNIFFER_H
#define WINSNIFFER_H

#include <string>

class WinSniffer
{
public:
    WinSniffer();
    void start();

private:
    uintptr_t sock = 0;

    void throwError(const std::string &msg);
};

#endif // WINSNIFFER_H
