// Эта программа получает доступ к очереди сообщений,
// отправляет в нее 5 текстовых сообщений с типом 1
// и одно пустое сообщение с типом 255, которое будет служить
// для второй программы сигналом прекращения работы.

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

// Отослать текстовое сообщение в очередь.
void sendText(message_queue& mq, const uint32_t priority, const string &text)
{
    // Готовим массив с байтами.
    vector<uint8_t> msg;
    msg.reserve(sizeof(MessageType) + text.size());

    // Указываем тип сообщения.
    msg.push_back(MessageType::Text);

    // Само сообщение - символы текстового сообщения.
    msg.insert(msg.end(), text.begin(), text.end());

    // Отправляем байты.
    mq.send(msg.data(), msg.size(), priority);
}

// Отослать целое 32-битное число в очередь.
void sendInteger32(message_queue& mq, const uint32_t priority, int32_t number)
{
    const auto numberPtr = reinterpret_cast<uint8_t*>(&number);

    // Готовим массив с байтами.
    vector<uint8_t> msgWithNumber;
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

int main()
{
    try
    {
        // Удалим предыдущую очередь сообщений, если была.
        message_queue::remove("lab11_mqueue");

        // Размер: 5 текстовых сообщений, 1 число, и 1 сигнал о завершении работы
        const size_t maxQueueSize = 7;
        const uint32_t priority = 1;
        const size_t maxMsgLength = sizeof(MessageType) + 100;

        // Создадим новую очередь сообщений.
        message_queue mq (create_only,
                          "lab11_mqueue",
                          maxQueueSize,
                          maxMsgLength
                          );

        // Отсылаем 5 сообщений в очередь.
        for (int i = 1; i <= 5; ++i)
        {
            sendText(mq, priority, "This is text message: " + to_string(i));
        }

        sendInteger32(mq, priority, -1'500'000'125);

        // Отсылаем сообщение о прекращении работы.
        auto endOfWorkMsg = MessageType::EndOfWork;
        mq.send(&endOfWorkMsg, sizeof(endOfWorkMsg), priority);
    }
    catch (interprocess_exception &ex)
    {
        // Сообщаем об ошибке.
        cout << ex.what() << endl;

        // Удалим очередь сообщений из-за ошибки.
        message_queue::remove("lab11_mqueue");

        return 1;
    }

    return 0;
}
