#ifndef SNIFFER_H
#define SNIFFER_H

#ifdef WIN32
#include "WinSniffer.h"
using Sniffer = WinSniffer;
#else
#include "LinuxSniffer.h"
using Sniffer = LinuxSniffer;
#endif

#endif // SNIFFER_H
