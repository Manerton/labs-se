#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

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

    // создаем пул потоков
    boost::asio::thread_pool pool(9);

    // используем лямбда-функцию для передачи параметра для функции
    // вместо std::bind или функционального объекта
    boost::asio::post(pool, [](){ func('A'); });
    boost::asio::post(pool, [](){ func('B'); });
    boost::asio::post(pool, [](){ func('C'); });
    boost::asio::post(pool, [](){ func('D'); });
    boost::asio::post(pool, [](){ func('E'); });
    boost::asio::post(pool, [](){ func('F'); });
    boost::asio::post(pool, [](){ func('G'); });
    boost::asio::post(pool, [](){ func('H'); });
    boost::asio::post(pool, [](){ func('I'); });
    // ожидаем все потоки в пуле
    pool.join();

    // заканчиваем считать время
    auto end = high_resolution_clock::now();
    cout << duration_cast<f_seconds>(end-start).count() << endl;

    system("pause");
    return 0;
}
