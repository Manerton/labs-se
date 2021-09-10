#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;

// float seconds
using f_seconds = duration<float, seconds::period>;

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
    thread t2 {func, 'B'};
    thread t3 {func, 'C'};
    thread t4 {func, 'D'};
    thread t5 {func, 'E'};
    thread t6 {func, 'F'};
    thread t7 {func, 'G'};
    thread t8 {func, 'H'};
    thread t9 {func, 'I'};
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
