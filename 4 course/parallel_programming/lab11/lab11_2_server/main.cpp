#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <map>
#include <sstream>
#include <memory>

#include <mqueue.h>

#include "../../posix_message_queue.hpp"

using namespace std;

/// Размер очереди сервера
static const size_t maxQueueSize = 10;
/// Дефолтный приоритет.
static const uint32_t priority = 1;
/// Размер одного сообщения.
static const size_t maxMsgLength = sizeof(pid_t) + 100;

using Connections = map<pid_t, unique_ptr<posix_message_queue>>;

/// Отослать текстовое сообщение в очередь.
void sendTextToClient(posix_message_queue& mq, const uint32_t priority, const std::string &text)
{
    // Отправляем байты.
    mq.send(text.data(), text.size(), priority);
}

void createNewConnection(pid_t clientId, Connections &connections)
{
    const string query_name = "lab11_2_client_" + to_string(clientId);

    // Откроем очередь сообщений.
    connections[clientId] = make_unique<posix_message_queue> (open_or_create_t(),
                                                              open_mode_t::write_only,
                                                              query_name.c_str(),
                                                              maxQueueSize,
                                                              maxMsgLength
                                                              );
}

int main()
{
    try
    {
        Connections connections;

        // Удалим старую очередь.
        posix_message_queue::remove("lab11_2_server");

        // Откроем очередь сообщений.
        posix_message_queue mq (create_only_t(),
                                open_mode_t::read_only,
                                "lab11_2_server",
                                maxQueueSize,
                                maxMsgLength
                                );

        cout << "> Max msg count in queue: " << mq.get_max_msg() << endl;
        cout << "> Max msg size in queue: " << mq.get_max_msg_size() << endl;

        while (true)
        {
            uint32_t recvd_priority;
            posix_message_queue::signed_size_type recvd_size;

            string request;
            request.resize(size_t(mq.get_max_msg_size()));

            mq.receive(request.data(), request.size(), recvd_size, recvd_priority);

            // Если что-то прочли
            if (recvd_size > 0)
            {
                // Размер буфера был максимально возможный, изменим его до количества прочитанных данных.
                request.resize(size_t(recvd_size));

                // Прочитаем Id клиента.
                pid_t clientId = 0;
                auto numberPtr = reinterpret_cast<char*>(&clientId);
                copy_n(request.begin(), sizeof(pid_t), numberPtr);

                // Прочитаем оставшееся сообщение.
                string msgText(request.begin()+sizeof(pid_t), request.end());

                cout << "[Client: " << clientId << "] " << msgText << endl;

                if (connections.count(clientId) == 0)
                {
                    createNewConnection(clientId, connections);
                }

                if (msgText != "end")
                {
                    const string response = "Server got your request: " + msgText;
                    sendTextToClient(*connections[clientId], priority, response);
                }
                else
                {
                    sendTextToClient(*connections[clientId], priority, "end");
                    connections.erase(clientId);
                }
            }
            else
            {
                throw runtime_error("Can't read message!");
            }
        }
    }
    catch (std::exception &ex)
    {
        // Сообщаем об ошибке.
        cout << "Error: " << ex.what() << endl;

        // Удалим очередь сообщений из-за ошибки.
        posix_message_queue::remove("lab11_2_server");

        return 1;
    }

    return 0;
}
