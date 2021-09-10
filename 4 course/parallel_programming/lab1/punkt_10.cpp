#include <thread>
#include <iostream>
#include <cmath>
#include <chrono>

#include <pthread.h>

using namespace std;
using namespace chrono;

// float seconds
using f_seconds = duration<float, seconds::period>;

// thread priority level
enum thread_priority
{
    idle = 1,
    lowest = 6,
    below_normal = 7,
    normal = 8,
    above_normal = 9,
    highest = 10,
    time_critical = 15
};

void setPriority(std::thread &th, thread_priority priority)
{
    sched_param sch;
    sch.sched_priority = priority;
    pthread_setschedparam(th.native_handle(), SCHED_OTHER, &sch);
}

void func(char label)
{
    double x = 12345.6789;
    for (int i = 0; i < 10000; i++)
    {
        for (int j = 0; j < 10000; j++)
        {
            x = sqrt(x);
            x = x + 0.000000001;
            x = pow(x, 2);
        }
    }
    cout << label << ": " << x << endl;
}

int main()
{
    // начинаем считать время
    auto start = high_resolution_clock::now();

    // создаем потоки
    thread t1 {func, 'A'};
    setPriority(t1, time_critical);
    thread t2 {func, 'B'};
    setPriority(t2, highest);
    thread t3 {func, 'C'};
    setPriority(t3, above_normal);
    thread t4 {func, 'D'};
    setPriority(t4, above_normal);
    thread t5 {func, 'E'};
    setPriority(t5, normal);
    thread t6 {func, 'F'};
    setPriority(t6, below_normal);
    thread t7 {func, 'G'};
    setPriority(t7, below_normal);
    thread t8 {func, 'H'};
    setPriority(t8, lowest);
    thread t9 {func, 'I'};
    setPriority(t9, idle);
    // ожидаем потоки
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();
    t9.join();

    // заканчиваем считать время
    auto end = high_resolution_clock::now();
    cout << duration_cast<f_seconds>(end-start).count() << endl;

    system("pause");
    return 0;
}
