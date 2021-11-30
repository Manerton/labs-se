#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <map>
#include <memory>

#include <mqueue.h>
#include <unistd.h>

#include "../../posix_message_queue.hpp"

using namespace std;

/// Размер очереди сервера
static const size_t maxQueueSize = 10;
/// Дефолтный приоритет.
static const uint32_t priority = 1;
/// Размер одного сообщения.
static const size_t maxMsgLength = sizeof(pid_t) + 100;

/// Отослать текстовое сообщение в очередь.
void sendTextToServer(posix_message_queue& mq, const uint32_t priority, pid_t clientId, const std::string &text)
{
    const auto numberPtr = reinterpret_cast<char*>(&clientId);

    // Готовим массив с байтами - Id клиента.
    std::vector<char> buffer(numberPtr, numberPtr + sizeof(pid_t));

    // Теперь вставляем туда наше текстовое сообщение.
    buffer.insert(buffer.end(), text.begin(), text.end());

    // Отправляем.
    mq.send(buffer.data(), buffer.size(), priority);
}

int main()
{
    pid_t id = getpid();
    const string query_name = "lab11_2_client_" + to_string(id);

    try
    {
        // Откроем очередь сообщений сервера.
        posix_message_queue mq_server (open_only_t(),
                                       open_mode_t::write_only,
                                       "lab11_2_server"
                                       );

        cout << "> Max msg count in server queue: " << mq_server.get_max_msg() << endl;
        cout << "> Max msg size in server queue: " << mq_server.get_max_msg_size() << endl;

        // Сообщаем серверу о том, что мы подключились.
        sendTextToServer(mq_server, priority, id, "Hello from client!");

        posix_message_queue mq (open_or_create_t(),
                                open_mode_t::read_only,
                                query_name.c_str(),
                                maxQueueSize,
                                maxMsgLength
                                );

        // Читаем ответы сервера.
        while (true)
        {
            cout << "> Messages remain: " << mq.get_num_msg() << endl;

            uint32_t recvd_priority;
            posix_message_queue::signed_size_type recvd_size;

            string msg;
            msg.resize(size_t(mq.get_max_msg_size()));

            mq.receive(msg.data(), msg.size(), recvd_size, recvd_priority);

            // Если что-то прочли
            if (recvd_size > 0)
            {
                // Размер буфера был максимально возможный, изменим его до количества прочитанных данных.
                msg.resize(size_t(recvd_size));

                if (msg == "end")
                {
                    cout << "End of work" << endl;
                    posix_message_queue::remove(query_name.c_str());
                    return 0;
                }

                cout << msg << endl;

                cout << "Your request: ";
                string newMsg;
                getline(cin, newMsg);
                sendTextToServer(mq_server, priority, id, newMsg);
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
        posix_message_queue::remove(query_name.c_str());

        return 1;
    }

    return 0;
}
