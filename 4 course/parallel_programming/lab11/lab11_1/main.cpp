#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>

#include "../../posix_message_queue.hpp"

using namespace std;

/// Типы сообщений и сигналы
enum MessageType : uint8_t
{
    Text = 0,
    Integer32 = 1,
    EndOfWork = 255
};

/// Размер: 5 текстовых сообщений, 1 число, и 1 сигнал о завершении работы
static const size_t maxQueueSize = 7;
/// Дефолтный приоритет.
static const uint32_t priority = 1;
/// Размер одного сообщения.
static const size_t maxMsgLength = sizeof(MessageType) + 100;

/// Отослать текстовое сообщение в очередь.
void sendText(posix_message_queue& mq, const uint32_t priority, const std::string &text)
{
    // Готовим массив с байтами.
    std::vector<char> msg;
    msg.reserve(sizeof(MessageType) + text.size());

    // Указываем тип сообщения.
    msg.push_back(MessageType::Text);

    // Само сообщение - символы текстового сообщения.
    msg.insert(msg.end(), text.begin(), text.end());

    // Отправляем байты.
    mq.send(msg.data(), msg.size(), priority);
}

/// Отослать целое 32-битное число в очередь.
void sendInteger32(posix_message_queue& mq, const uint32_t priority, int32_t number)
{
    const auto numberPtr = reinterpret_cast<uint8_t*>(&number);

    // Готовим массив с байтами.
    std::vector<char> msgWithNumber;
    msgWithNumber.reserve(sizeof(MessageType) + sizeof(int32_t));

    // Указываем тип сообщение.
    msgWithNumber.push_back(MessageType::Integer32);

    // Само сообщение - это байты от числа.
    msgWithNumber.insert(msgWithNumber.end(),
                         numberPtr,
                         numberPtr + sizeof(int32_t)
                         );

    // Отправляем байты.
    mq.send(msgWithNumber.data(), msgWithNumber.size(), priority);
}

/// Отослать сигнал (например "конец работы")
void sendSignal(posix_message_queue& mq, MessageType signal)
{
    const int32_t priority = 0;
    mq.send(reinterpret_cast<char*>(&signal), sizeof(signal), priority);
}

void readerWork(const string &query_name)
{
    // Откроем очередь сообщений.
    posix_message_queue mq (open_or_create_t(),
                            open_mode_t::read_only,
                            query_name.c_str(),
                            maxQueueSize,
                            maxMsgLength
                            );

    cout << "> Max msg count in queue: " << mq.get_max_msg() << endl;
    cout << "> Max msg size in queue: " << mq.get_max_msg_size() << endl;

    while (true)
    {
        cout << "> Messages remain: " << mq.get_num_msg() << endl;

        uint32_t priority;
        posix_message_queue::signed_size_type recvd_size;

        vector<char> msg;
        msg.resize(size_t(mq.get_max_msg_size()));

        mq.receive(msg.data(), msg.size(), recvd_size, priority);

        const auto type = MessageType(msg[0]);

        // Если что-то прочли и это не конец работы
        if (recvd_size > 0 && type != MessageType::EndOfWork)
        {
            if (type == MessageType::Text)
            {
                string str(msg.begin()+1, msg.end());
                cout << str << endl;
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
                cout << "> Got message with unknown type!" << endl;
            }

        }
        else
        {
            cout << "Got 'end of work' signal!" << endl;
            cout << "> End of work!" << endl;

            posix_message_queue::remove(query_name.c_str());

            return;
        }
    }
}

int main(int argc, char *argv[])
{
    string mqueue_writer_name = "lab11_mqueue_a";
    string mqueue_reader_name = "lab11_mqueue_b";

    if (argc == 2)
    {
        string arg = argv[1];

        if (arg == "b")
        {
            swap(mqueue_reader_name, mqueue_writer_name);
        }
    }

    try
    {
        thread readerThread {readerWork, ref(mqueue_reader_name)};

        // Создадим или откроем новую очередь сообщений.
        posix_message_queue mq (open_or_create_t(),
                                open_mode_t::write_only,
                                mqueue_writer_name.c_str(),
                                maxQueueSize,
                                maxMsgLength);

        // Отсылаем 5 сообщений в очередь.

        sendText(mq, priority, "Hi from " + mqueue_writer_name);

        for (int i = 2; i <= 5; ++i)
        {
            sendText(mq, priority, "This is text message: " + to_string(i));
        }

        sendInteger32(mq, priority, -1'500'000'125);

        // Отсылаем сообщение о прекращении работы.
        auto endOfWorkMsg = MessageType::EndOfWork;
        sendSignal(mq, endOfWorkMsg);

        readerThread.join();
    }
    catch (std::exception &ex)
    {
        // Сообщаем об ошибке.
        cout << ex.what() << endl;

        // Удалим очередь сообщений из-за ошибки.
        posix_message_queue::remove(mqueue_writer_name.c_str());

        return 1;
    }

    return 0;
}
