#ifndef WINSNIFFER_H
#define WINSNIFFER_H

#include <string>

class WinSniffer
{
public:
    WinSniffer();
private:
    void throwError(const std::string &msg);
};

#endif // WINSNIFFER_H
