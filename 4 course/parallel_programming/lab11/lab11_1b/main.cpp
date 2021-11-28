// Эта программа получает доступ к очереди сообщений,
// и читает из неё сообщения с любым типом в порядке FIFO до тех пор,
// пока не получит сообщение с типом 255,
// которое будет служить сигналом прекращения работы.

#include <iostream>
#include <algorithm>
#include <vector>
#include <exception>

#include "../../posix_message_queue.hpp"

using namespace std;

enum MessageType : uint8_t
{
    Text = 0,
    Integer32 = 1,
    EndOfWork = 255
};

int main()
{
    try
    {
        // Создадим новую очередь сообщений.
        posix_message_queue mq (open_only_t(),
                                open_mode_t::read_only,
                                "lab11_mqueue"
                                );

        cout << "Max msg count in queue: " << mq.get_max_msg() << endl;
        cout << "Max msg size in queue: " << mq.get_max_msg_size() << endl;

        while (true)
        {
            cout << "Messages remain: " << mq.get_num_msg() << endl;

            uint32_t priority;
            posix_message_queue::signed_size_type recvd_size;

            vector<char> msg;
            msg.resize(size_t(mq.get_max_msg_size()));

            mq.receive(msg.data(), msg.size(), recvd_size, priority);

            const auto type = MessageType(msg[0]);

            if (recvd_size > 0 && type != MessageType::EndOfWork)
            {
                if (type == MessageType::Text)
                {
                    string str(msg.begin()+1, msg.end());
                    cout << str << " " << str.length() << endl;
                }
                else if (type == MessageType::Integer32)
                {
                    int32_t number = 0;
                    auto numberPtr = reinterpret_cast<char*>(&number);
                    copy_n(msg.begin()+1, sizeof(int32_t), numberPtr);
                    cout << number << endl;
                }
                else
                {
                    cout << "Got message with unknown type!" << endl;
                }

            }
            else
            {
                cout << "End of work!" << endl;

                posix_message_queue::remove("lab11_mqueue");

                return 0;
            }
        }
    }
    catch (std::exception &ex)
    {
        // Сообщаем об ошибке.
        cout << "Error: " << ex.what() << endl;

        // Удалим очередь сообщений из-за ошибки.
        posix_message_queue::remove("lab11_mqueue");

        return 1;
    }

    return 0;
}
