#include <thread>
#include <iostream>
#include <atomic>
#include <mutex>

using namespace std;

/// Переменная a является глобальной статической для всей программы,
/// поэтому она будет разделяться обеими нитями исполнения.
int a = 0;

/// Блокировка для корректного вывода и избежания гонки.
mutex m;

/// Работа потока.
void threadWork()
{
    scoped_lock lck {m};
    auto myThId = this_thread::get_id();
    cout << "Thread " << myThId << ", Calculation result = " << ++a << endl;
}

int main()
{
    try
    {
        cout << "Main threadId: " << this_thread::get_id() << endl;

        // Создаем новый поток.
        auto th1 = thread( threadWork );

        {
            scoped_lock lck {m};
            cout << "Thread created, threadId = " << th1.get_id() << endl;
        }

        // Создаем еще один поток.
        auto th2 = thread( threadWork );

        {
            scoped_lock lck {m};
            cout << "Thread created, threadId = " << th2.get_id() << endl;
        }

        // Сами выполняем работу.
        threadWork();

        // Ждем когда отработают другие.
        th1.join();
        th2.join();
    }
    catch (std::exception& ex)
    {
        cout << "Error: " << ex.what() << endl;
    }

    return 0;
}
