#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <sstream>

using namespace std;
using namespace chrono;

using d_seconds = duration<double, seconds::period>;
using d_milliseconds = duration<double, milliseconds::period>;

using index = size_t;

template <typename C>
string displayContainer(const C& container)
{
    typename C::const_iterator it;

    stringstream ss;

    for (it = container.begin(); it != container.end(); ++it)
    {
        ss << *it << " ";
    }

    ss << endl;

    return ss.str();
}

/// классический метод решета Эратосфена (базовые простые числа)
vector<index> getBasePrimeBySieveOfEratosthenes(index n)
{
    vector<index> basePrime;

    vector<bool> range(n+1, false);
    for (index i = 2; i <= n; ++i)
    {
        // если непомеченное простое число
        if (!range[i])
        {
            basePrime.push_back(i);
            // пометим другие числа на расстоянии шага i как составные
            for (index j = i + i; j <= n; j += i)
            {
                range[j] = true;
            }
        }
    }

    return basePrime;
}

int main(int argc, char *argv[])
{
    index n = 50;
    size_t launchCount = 1;

    if (argc >= 2)
    {
        n = stoull(argv[1]);
        launchCount = stoull(argv[2]);
    }

    vector<double> times(launchCount);

    vector<size_t> basePrime;

    for (size_t i = 0; i < launchCount; ++i)
    {
        // начинаем считать время
        auto start = steady_clock::now();

        // что-то вычисляем
        basePrime = getBasePrimeBySieveOfEratosthenes(n);

        // заканчиваем считать время
        auto end = steady_clock::now();

        times[i] = duration_cast<d_milliseconds>(end-start).count();
    }


    double sum = accumulate(times.begin(), times.end(), 0.0);
    sum /= double(launchCount);

    cout << "First step (" << launchCount << " launch): " << sum << " ms" << endl;

    system("pause");
    return 0;
}
