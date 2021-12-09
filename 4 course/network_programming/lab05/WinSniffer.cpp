#include "WinSniffer.h"


#include <iostream>
#include <string>
#include <algorithm>

#include <winsock2.h>
#include <inaddr.h>
#include <mstcpip.h>

using namespace std;

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
    auto sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (sock == INVALID_SOCKET)
    {
        throwError("Failed to create raw socket.");
    }

    // Определяем название хоста.
    string hostname(100, ' ');
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
    dest.sin_port = 0;

    // Привязываем сокет к этому адресу.
    cout << "> Binding socket to local address and port 0..." << endl;
    if (bind(sock, reinterpret_cast<sockaddr*>(&dest), sizeof(dest)) == SOCKET_ERROR)
    {
        throwError("Bind failed.");
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
        throwError("ioctl failed.");
    }

    for (int i = 0; i < 10; ++i)
    {
        array<char, 65535> buf {};
        auto size = recvfrom(sock, buf.data(), 65535, 0, nullptr, nullptr);
        cout.write(buf.data(), size);
    }
}

void WinSniffer::throwError(const std::string &msg)
{
    throw runtime_error(msg + " | Error code: " + to_string(WSAGetLastError()));
}
