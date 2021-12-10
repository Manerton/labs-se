#ifdef WIN32
#include "WinSniffer.h"
#include "PacketParser.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <limits>

#include <winsock2.h>
#include <inaddr.h>
#include <mstcpip.h>

using namespace std;

void WinSniffer::throwError(const std::string &msg)
{
    throw runtime_error(msg + " | Error code: " + to_string(WSAGetLastError()));
}

WinSniffer::WinSniffer()
{
    // Инициализируем Winsock2.
    cout << "> Init Winsock2..." << endl;
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0)
    {
        throwError("WSAStartup failed.");
    }

    // Создаем сырой сокет.
    cout << "> Creating RAW socket..." << endl;
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (sock == INVALID_SOCKET)
    {
        throwError("Failed to create raw socket.");
    }

    // Определяем название хоста.
    const size_t hostname_len = 100;
    string hostname(hostname_len, ' ');
    if (gethostname(hostname.data(), int(hostname.size())) == SOCKET_ERROR)
    {
        throwError("Gethostname error.");
    }
    cout << "> Hostname: " << hostname << endl;

    // Получаем сетевые интерфейсы по имени хоста.
    auto local = gethostbyname(hostname.c_str());
    cout << "> Available network interfaces: " << endl;
    if (local == nullptr)
    {
        throwError("Can't get IPs of the localhost.");
    }

    // Выводим все локальные сетевые интерфейсы.
    vector<in_addr> interfaces;
    for (int i = 0; local->h_addr_list[i] != nullptr; ++i)
    {
        in_addr addr {};
        copy_n(local->h_addr_list[i], sizeof(in_addr), reinterpret_cast<char*>(&addr));
        cout << " - Interface number: " << i << " | " << "Address: " << inet_ntoa(addr) << endl;
        interfaces.push_back(addr);
    }

    size_t ch = 0;
    // Если их больше одного, то предлагаем выбрать один из них.
    if (interfaces.size() > 1)
    {
        cout << "Choose the interface number to sniff: ";
        cin >> ch;
    }

    // Структура - локальный IP адрес выбранного сетевого интерфейса.
    sockaddr_in dest {};
    dest.sin_addr = interfaces[ch];
    dest.sin_family = AF_INET;

    // Привязываем сокет к этому адресу.
    cout << "> Binding socket to local address..." << endl;
    if (bind(sock, reinterpret_cast<sockaddr*>(&dest), sizeof(dest)) == SOCKET_ERROR)
    {
        throwError("bind() failed.");
    }

    cout << "> Try to enable SIO_RCVALL..." << endl;
    DWORD flag = RCVALL_ON;
    // Включаем возможность сокету принимать пакеты проходящие через сетевой интерфейс.
    if (WSAIoctl(sock,
                 SIO_RCVALL,
                 &flag, sizeof(flag),
                 nullptr, 0,
                 reinterpret_cast<LPDWORD>(&ch),
                 nullptr , nullptr
                 ) == SOCKET_ERROR
            )
    {
        throwError("WSAIoctl() failed.");
    }
}

void WinSniffer::start()
{
    const size_t packageSize = 65536;

    vector<char> buf(packageSize);
    while (true)
    {
        auto recvSize = recvfrom(sock, buf.data(), packageSize, 0, nullptr, nullptr);
        if (recvSize > 0)
        {
            cout << endl << "//----------------------------------------------//" << endl << endl;
            cout << PacketParser::parsePacket(buf) << endl;
        }
        else
        {
            throwError("recvfrom() failed.");
        }
    }
}
#endif
