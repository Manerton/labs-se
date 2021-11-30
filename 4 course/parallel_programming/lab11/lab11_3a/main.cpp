// В этой программе реализуется бинарный семафор на основе очереди сообщений.
// Если в очереди отсутствует сообщение, процесс блокируется и ждёт сообщение.
// Как только в очереди появляется сообщение, процесс его считывает и процесс разблокируется.
// Соответственно send и receive в message queue - это post и wait в семафорах.

#include <iostream>
#include <string>
#include <algorithm>

#include "../../posix_message_queue.hpp"

using namespace std;

int main()
{
    try
    {
        posix_message_queue mq (open_or_create_t(),
                                open_mode_t::read_and_write,
                                "lab11_3", 1, 1);

        cout << "Semaphore waits (message queue empty)." << endl;

        char message_signal = 0;
        posix_message_queue::signed_size_type recvd_size = 0;
        uint32_t priority = 0;
        mq.receive(&message_signal, 1, recvd_size, priority);

        cout << "Semaphore unlocked!" << endl;

        posix_message_queue::remove("lab11_3");
    }
    catch (exception &ex)
    {
        cout << "Error: " << ex.what() << endl;
        return 1;
    }

    return 0;

}
