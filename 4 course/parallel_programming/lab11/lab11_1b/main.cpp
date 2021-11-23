// Эта программа получает доступ к очереди сообщений,
// и читает из неё сообщения с любым типом в порядке FIFO до тех пор,
// пока не получит сообщение с типом 255,
// которое будет служить сигналом прекращения работы.

#include <iostream>

#include <boost/interprocess/ipc/message_queue.hpp>

using namespace std;
using namespace boost::interprocess;

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
        message_queue mq (open_only,
                          "lab11_mqueue"
                          );

        while (true)
        {
            uint32_t priority;
            message_queue::size_type recvd_size;

            vector<uint8_t> msg;
            msg.resize(mq.get_max_msg_size());

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
                    auto numberPtr = reinterpret_cast<uint8_t*>(&number);
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

                //message_queue::remove("lab11_mqueue");

                return 0;
            }
        }
    }
    catch (interprocess_exception &ex)
    {
        // Сообщаем об ошибке.
        cout << "Error: " << ex.what() << endl;

        // Удалим очередь сообщений из-за ошибки.
        message_queue::remove("lab11_mqueue");

        return 1;
    }

    return 0;
}
