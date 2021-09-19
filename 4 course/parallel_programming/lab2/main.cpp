#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>

using namespace std;
using namespace chrono;

using d_seconds = duration<double, seconds::period>;

double single_calc(double arg)
{
    for (int i = 0; i < 1000; ++i)
    {
        arg += sqrt(arg);
        arg += 0.000000001;
        arg += pow(arg, 0.034);
        arg += pow(arg, 0.118);
    }
    return arg;
}

void seq_func(vector<double> &arr, size_t i_begin, size_t i_end, size_t K)
{
    for (size_t i = i_begin; i < i_end; ++i)
    {
        for (size_t j = 0; j < K; ++j)
        {
            arr[i] = single_calc(arr[i]);
        }
    }
}

void par_func(vector<double> &arr, size_t K, size_t M)
{
    double range = double(arr.size()) / double(M);

    vector<thread> threads;

    for (size_t i = 0; i < M; ++i)
    {
        const auto i_begin = size_t(llround(double(i) * range));
        const auto i_end = size_t(llround(double(i) * range + range));
        thread t {seq_func, ref(arr), i_begin, i_end, K};
        threads.push_back(move(t));
    }

    for (auto &t : threads) t.join();
}

void unbalanced_seq_func(vector<double> &arr, size_t i_begin, size_t i_end, size_t step)
{
    for (size_t i = i_begin; i < i_end; i += step)
    {
        for (size_t j = 0; j < i; ++j)
        {
            arr[i] = single_calc(arr[i]);
        }
    }
}

void unbalanced_par_func(vector<double> &arr, size_t M)
{
    double range = double(arr.size()) / double(M);

    vector<thread> threads;

    for (size_t i = 0; i < M; ++i)
    {
        const auto i_begin = size_t(llround(double(i) * range));
        const auto i_end = size_t(llround(double(i) * range + range));
        thread t {unbalanced_seq_func, ref(arr), i_begin, i_end, 1};
        threads.push_back(move(t));
    }

    for (auto &t : threads) t.join();
}

void circle_par_func(vector<double> &arr, size_t M)
{
    vector<thread> threads;

    for (size_t i = 0; i < M; ++i)
    {
        thread t {unbalanced_seq_func, ref(arr), i, arr.size(), M};
        threads.push_back(move(t));
    }

    for (auto &t : threads) t.join();
}

int main()
{
    // рандом (генерит числа от 0 до 4294967295)
    std::mt19937 mt(static_cast<mt19937::result_type>(time(nullptr)));

    // создаем вектор на N случайных элементов
    const size_t N = 10;
    const size_t K = 1;
    const size_t M = 3;
    vector<double> arr(N);
    generate(arr.begin(), arr.end(), mt);

    // начинаем считать время
    auto start = high_resolution_clock::now();

    // последовательное выполнение
    // seq_func(arr, 0, N-1, K);

    // параллельное выполнение
    // par_func(arr, K, M);

    // несбалансированное параллельное выполнение (с разделением по диапазону)
    // unbalanced_par_func(arr, M);

    // сбалансированное параллельное выполнение (с круговым разделением)
    circle_par_func(arr, M);

    // заканчиваем считать время
    auto end = high_resolution_clock::now();
    cout << duration_cast<d_seconds>(end-start).count() << endl;

    system("pause");
    return 0;
}
