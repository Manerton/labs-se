#ifndef WIN32
#include "LinuxSniffer.h"
#include "PacketParser.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <limits>

#include <sys/socket.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <arpa/inet.h>

using namespace std;

void LinuxSniffer::throwError(const std::string &msg)
{
    throw runtime_error(msg + " | Error code: " + to_string(errno));
}

LinuxSniffer::LinuxSniffer()
{
    // Создаем сырой сокет, который перехватывает входящие
    // и исходящие пакеты (причем любого типа).
    cout << "> Creating RAW socket..." << endl;
    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock == -1)
    {
        throwError("Failed to create raw socket.");
    }

    // Вот так можно забиндить на какой-нибудь определенный сетевой интерфейс.
    //setsockopt(sock_raw , SOL_SOCKET , SO_BINDTODEVICE , "eth0" , strlen("eth0")+ 1 );
}

void LinuxSniffer::start()
{
    const size_t packageSize = 65536;

    vector<char> buf(packageSize);
    while (true)
    {
        auto recvSize = recvfrom(sock, buf.data(), packageSize, 0, nullptr, nullptr);
        if (recvSize > 0)
        {
            cout << endl << "//----------------------------------------------//" << endl << endl;

            // Пропустим заголовок Ethernet фрейма.
            vector<char> bufWithoutEth(buf.begin() + sizeof(ethhdr), buf.end());
            cout << PacketParser::parsePacket(bufWithoutEth) << endl;
        }
        else
        {
            throwError("recvfrom() failed.");
        }
    }
}
#endif
