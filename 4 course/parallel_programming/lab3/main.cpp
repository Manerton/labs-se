#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <sstream>
#include <fstream>
#include <list>

using namespace std;
using namespace chrono;

using d_seconds = duration<double, seconds::period>;
using d_milliseconds = duration<double, milliseconds::period>;

using FirstStepResult = vector<size_t>;
using SecondStepResult = vector<uint64_t>;

#define debug

/// Вывести содержимое контейнера (где есть begin и end).
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

/// Бенчмарк.
template <typename Result, typename Function, typename... Args>
pair<double, Result> timeBenchmark(const Function& func, const Args&... args)
{
    Result calc;

    // начинаем считать время
    auto start = steady_clock::now();

    // что-то вычисляем
    calc = func(args...);

    // заканчиваем считать время
    auto end = steady_clock::now();

    auto res = duration_cast<d_milliseconds>(end-start).count();

    return make_pair(res, calc);
}

template <typename T>
void writeToFile(const string& filename, const T& data)
{
    ofstream f(filename);

    if (f)
    {
        for (auto c: data)
        {
            f << c << " ";
        }
    }
}

/// Классический метод решета Эратосфена (базовые простые числа).
FirstStepResult getBasePrimeBySieveOfEratosthenes(size_t n)
{
    FirstStepResult basePrime;

    vector<bool> range(n+1, false);
    for (size_t i = 2; i <= n; ++i)
    {
        // если непомеченное простое число
        if (!range[i])
        {
            basePrime.push_back(i);
            // пометим другие числа на расстоянии шага i как составные
            for (size_t j = i + i; j <= n; j += i)
            {
                range[j] = true;
            }
        }
    }

    return basePrime;
}

/// Последовательная декомпозиция по данным
SecondStepResult seqDecompositionByData(uint64_t i_start,
                                        uint64_t i_end,
                                        const FirstStepResult &basePrime
                                        )
{
    SecondStepResult primeNumbers;

    for (uint64_t number = i_start + 1; number <= i_end; ++number)
    {
        auto isDivide = [&](uint64_t basePrimeNumber)
        {
            return number % basePrimeNumber == 0;
        };

        const auto res = find_if(basePrime.begin(), basePrime.end(), isDivide);

        if (res == basePrime.end())
        {
            primeNumbers.push_back(number);
        }
    }

    return primeNumbers;
}

/// Параллельная декомпозиция по данным
SecondStepResult parDecompositionByData(uint64_t begin,
                                        uint64_t n,
                                        const FirstStepResult &basePrime,
                                        uint8_t threadCount
                                        )
{
#ifdef debug
    cout << "> Par decomposition by data [" << int(threadCount) << " threads] start..." << endl;
#endif

    /// результат - массив с простыми числами из диапазона от (begin, n]
    SecondStepResult prime;

    list<SecondStepResult> results;

    double range = double(n - begin) / double(threadCount);

#ifdef debug
    cout << "data range: " << range << endl;
#endif

    list<thread> threads;

    for (size_t i = 0; i < threadCount; ++i)
    {
        const auto i_begin = uint64_t(llround(double(i) * range)) + begin;
        const auto i_end = uint64_t(llround(double(i) * range + range)) + begin;

#ifdef debug
        cout << i << " thread - " << "begin: " << i_begin << " " << "end: " << i_end << endl;
#endif

        const auto threadHandler = [i_begin, i_end, &basePrime, &results]()
        {
            results.push_back(seqDecompositionByData(i_begin, i_end, ref(basePrime)));
        };

        thread t { threadHandler };
        threads.push_back(move(t));
    }

    for (auto &t : threads)
    {
        t.join();
    }

    results.sort([](const SecondStepResult& v1, const SecondStepResult& v2)
    {
        return v1.front() < v2.front();
    });

    for (const auto &res: results)
    {
        prime.insert(prime.end(), res.begin(), res.end());
    }

    return prime;
}

int main(int argc, char *argv[])
{
    constexpr size_t default_n = 100;

    auto n = default_n;

    if (argc >= 2)
    {
        n = stoull(argv[1]);
    }

    const auto sqrtN = static_cast<size_t>(sqrt(n));

    // Первый шаг.
    const auto firstStepRes = timeBenchmark<FirstStepResult>(getBasePrimeBySieveOfEratosthenes, sqrtN);
    cout << "> First step: " << firstStepRes.first << " ms" << endl;
    writeToFile("firstStep.txt", firstStepRes.second);

    // Последовательная декомпозиция по данным
    const auto seqDecompositionByDataRes = timeBenchmark<SecondStepResult>(seqDecompositionByData, sqrtN, n, ref(firstStepRes.second));
    cout << "> Seq decomposition by data: " << seqDecompositionByDataRes.first << " ms" << endl;
    writeToFile("SeqDecompositionByData.txt", seqDecompositionByDataRes.second);

    // Параллельная декомпозиция по данным: 2 потока
    const auto parDecompositionByDataRes_2t = timeBenchmark<SecondStepResult>(parDecompositionByData, sqrtN, n, ref(firstStepRes.second), uint8_t(2));
    cout << "> Par decomposition by data [2 threads]: " << parDecompositionByDataRes_2t.first << " ms" << endl;
    writeToFile("ParDecompositionByData_2t.txt", parDecompositionByDataRes_2t.second);

    // Параллельная декомпозиция по данным: 4 потока
    const auto parDecompositionByDataRes_4t = timeBenchmark<SecondStepResult>(parDecompositionByData, sqrtN, n, ref(firstStepRes.second), uint8_t(4));
    cout << "> Par decomposition by data [4 threads]: " << parDecompositionByDataRes_4t.first << " ms" << endl;
    writeToFile("ParDecompositionByData_4t.txt", parDecompositionByDataRes_4t.second);

    // Параллельная декомпозиция по данным: 8 потоков
    const auto parDecompositionByDataRes_8t = timeBenchmark<SecondStepResult>(parDecompositionByData, sqrtN, n, ref(firstStepRes.second), uint8_t(8));
    cout << "> Par decomposition by data [8 threads]: " << parDecompositionByDataRes_8t.first << " ms" << endl;
    writeToFile("ParDecompositionByData_8t.txt", parDecompositionByDataRes_8t.second);

    system("pause");
    return 0;
}
