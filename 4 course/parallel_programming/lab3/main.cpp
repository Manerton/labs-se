/** \defgroup main Главный модуль программы
    @{
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <sstream>
#include <fstream>
#include <list>
#include <mutex>

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

//#define debug

using namespace std;
using namespace chrono;

/// Миллисекунды в формате дробного числа.
using d_milliseconds = duration<double, milliseconds::period>;

/// Массив с базовыми простыми числами (результат первого этапа алгоритма).
using FirstStepResult = vector<size_t>;

/// Массив с найденными простыми числами в ходе выполнения второго этапа алгоритма.
using SecondStepResult = vector<uint64_t>;

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

/**
 * @brief Бенчмарк.
 * @param func Выполняемая функция.
 * @param args Аргументы выполняемой функции.
 * @return Результат вычисления и время выполнения функции.
 */
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

    auto time = duration_cast<d_milliseconds>(end-start).count();

    return make_pair(time, calc);
}

/// Запись данных в файл.
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

/**
 * @brief Первый этап модицифированного последовательного поиска простых чисел.
 * @details
 * Классический метод - решето Эратосфена.
 * Поиск в интервале [2; n]
 * @param n Верхняя граница поиска простых чисел.
 * @return Массив с базовыми простыми числами.
 */
FirstStepResult getBasePrimeBySieveOfEratosthenes(const size_t n)
{
    FirstStepResult basePrime;

    vector<uint8_t> range(n+1, 0);
    for (size_t i = 2; i <= n; ++i)
    {
        // если непомеченное простое число
        if (!range[i])
        {
            basePrime.push_back(i);
            // пометим другие числа на расстоянии шага i как составные
            for (size_t j = i + i; j <= n; j += i)
            {
                range[j] = 1;
            }
        }
    }

    return basePrime;
}


/**
 * @brief Второй этап модифицированного последовательного поиска простых чисел.
 * @param i_begin Начальный индекс поиска
 * @param i_end Конечный индекс поиска
 * @param basePrime Массив с базовыми простыми числами
 * @return Массив с простыми числами, найденные в диапазоне от (i_begin; i_end]
 */
SecondStepResult seqModSearch(const uint64_t i_begin,
                              const uint64_t i_end,
                              const FirstStepResult &basePrime
                              )
{
    SecondStepResult primeNumbers;

    for (uint64_t number = i_begin + 1; number <= i_end; ++number)
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

/**
 * @brief Второй этап модифицированного последовательного поиска простых чисел в полном диапазоне.
 * @param[in] i_begin Начальный индекс поиска
 * @param[in] i_end Конечный индекс поиска
 * @param[in] basePrime Массив с базовыми простыми числами
 * @param[in] basePrimeBegin Начальный индекс массива с базовыми простыми числами
 * @param[in] basePrimeEnd Конечный индекс массива с базовыми простыми числами
 * @param[out] fullRange Полный диапазон чисел, где 0 - простое, 1 - составное число.
 */
void seqModSearchInFullRange(const uint64_t i_begin,
                             const uint64_t i_end,
                             const FirstStepResult &basePrime,
                             const ptrdiff_t basePrimeBegin,
                             const ptrdiff_t basePrimeEnd,
                             vector<uint8_t> &fullRange
                             )
{
    uint64_t firstNumber = i_begin + 1;
    for (uint64_t number = firstNumber; number <= i_end; ++number)
    {
        auto isDivide = [&](uint64_t basePrimeNumber)
        {
            return number % basePrimeNumber == 0;
        };

        const auto res = find_if(basePrime.begin() + basePrimeBegin, basePrime.begin() + basePrimeEnd, isDivide);

        if (res != basePrime.begin() + basePrimeEnd)
        {
            fullRange[number - firstNumber] = 1;
        }
    }
}

/**
 * @brief Второй этап модифицированного последовательного поиска простых чисел в полном диапазоне по одному простому числу.
 * @param[in] i_begin Начальный индекс поиска
 * @param[in] i_end Конечный индекс поиска
 * @param[in] prime Простое число
 * @param[out] fullRange Полный диапазон чисел, где 0 - простое, 1 - составное число.
 */
void seqModSearchInFullRangeByOnePrime(const uint64_t i_begin,
                                       const uint64_t i_end,
                                       const size_t prime,
                                       vector<uint8_t> &fullRange
                                       )
{
    uint64_t firstNumber = i_begin + 1;
    for (uint64_t number = firstNumber; number <= i_end; ++number)
    {
        if (number % prime == 0)
        {
            fullRange[number - firstNumber] = 1;
        }
    }
}

/// Параллельная декомпозиция по данным.
SecondStepResult parDecompositionByData(uint64_t begin,
                                        uint64_t n,
                                        const FirstStepResult &basePrime,
                                        uint8_t threadCount
                                        )
{
#ifdef debug
    cout << "> Par decomposition by data [" << int(threadCount) << " threads] start..." << endl;
#endif

    // Результат - массив с простыми числами из диапазона от (begin, n].
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
            results.push_back(seqModSearch(i_begin, i_end, ref(basePrime)));
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

/// Последовательная декомпозиция набора простых чисел.
SecondStepResult seqDecompositionByBasePrime(const uint64_t begin,
                                             const uint64_t n,
                                             const FirstStepResult &basePrime
                                             )
{
    const auto diff = n - begin;
    // диапазон чисел (begin, n]
    // 1  - если составное
    // 0 - если простое
    vector<uint8_t> fullRange(diff, 0);

    // Результат - массив с простыми числами из диапазона от (begin, n].
    SecondStepResult prime;

    seqModSearchInFullRange(begin, n, basePrime, ptrdiff_t(0), ptrdiff_t(basePrime.size()-1), fullRange);

    for (size_t i = 0; i < diff; ++i)
    {
        if (!fullRange[i])
        {
            prime.push_back(i + (begin + 1));
        }
    }

    return prime;
}

/// Параллельная декомпозиция набора простых чисел.
SecondStepResult parDecompositionByBasePrime(const uint64_t begin,
                                             const uint64_t n,
                                             const FirstStepResult &basePrime,
                                             const uint8_t threadCount
                                             )
{

#ifdef debug
    cout << "> Par decomposition by base prime [" << int(threadCount) << " threads] start..." << endl;
#endif

    const auto diff = n - begin;
    // диапазон чисел (begin, n]
    // 1  - если составное
    // 0 - если простое
    vector<uint8_t> fullRange(diff, 0);

    // Результат - массив с простыми числами из диапазона от (begin, n].
    SecondStepResult prime;

    double rangeForThread = double(basePrime.size()) / double(threadCount);

#ifdef debug
    cout << "base prime range for thread: " << rangeForThread << endl;
#endif

    list<thread> threads;

    for (size_t i = 0; i < threadCount; ++i)
    {
        const auto i_begin = ptrdiff_t(llround(double(i) * rangeForThread));
        const auto i_end = ptrdiff_t(llround(double(i) * rangeForThread + rangeForThread));

#ifdef debug
        cout << i << " thread - " << "begin: " << i_begin << " " << "end: " << i_end << endl;
#endif

        if (i_begin == i_end)
        {

#ifdef debug
            cout << "skip thread, because i_begin == i_end" << endl;
#endif
            continue;
        }

        thread t { seqModSearchInFullRange, begin, n, ref(basePrime), i_begin, i_end, ref(fullRange) };
        threads.push_back(move(t));
    }

    for (auto &t : threads)
    {
        t.join();
    }

    for (size_t i = 0; i < diff; ++i)
    {
        if (!fullRange[i])
        {
            prime.push_back(i + (begin + 1));
        }
    }

    return prime;
}

/// Параллельный алгоритм с использованием Thread Pool
SecondStepResult parThreadPool(uint64_t begin,
                               uint64_t n,
                               const FirstStepResult &basePrime,
                               const uint8_t threadCount
                               )
{

#ifdef debug
    cout << "> Par thread pool start..." << endl;
#endif

    const auto diff = n - begin;
    // диапазон чисел (begin, n]
    // 1  - если составное
    // 0 - если простое
    vector<uint8_t> fullRange(diff, 0);

    // Результат - массив с простыми числами из диапазона от (begin, n].
    SecondStepResult prime;

    const auto basePrimeSize = basePrime.size();

    boost::asio::thread_pool pool(threadCount);

    for (size_t i = 0; i < basePrimeSize; ++i)
    {
        const auto prime = basePrime[i];

        boost::asio::post(pool, [prime, begin, n, &fullRange]()
        {
            seqModSearchInFullRangeByOnePrime(begin, n, prime, fullRange);
        });

    }

    // ожидаем все потоки в пуле
    pool.join();

    for (size_t i = 0; i < diff; ++i)
    {
        if (!fullRange[i])
        {
            prime.push_back(i + (begin + 1));
        }
    }

    return prime;
}

/// Параллельный алгоритм с использованием Thread Pool
SecondStepResult parPrimeEnumeration(uint64_t begin,
                                     uint64_t n,
                                     const FirstStepResult &basePrime,
                                     const uint8_t threadCount
                                     )
{

#ifdef debug
    cout << "> Par prime enumeration [" << int(threadCount) << " threads] start..." << endl;
#endif

    const auto diff = n - begin;
    // диапазон чисел (begin, n]
    // 1  - если составное
    // 0 - если простое
    vector<uint8_t> fullRange(diff, 0);

    const auto basePrimeSize = basePrime.size();

    list<thread> threads;

    size_t currentPrimeIndex = 0;
    mutex m;

    for (size_t i = 0; i < threadCount; ++i)
    {
        auto handler = [begin, n, basePrimeSize, &basePrime, &m, &currentPrimeIndex, &fullRange]()
        {
            while (true)
            {
                unique_lock lck {m};

                if (currentPrimeIndex >= basePrimeSize)
                {
                    break;
                }

                auto prime = basePrime[currentPrimeIndex++];

                // разблокируем мьютекс, так как перестали работать
                // с разделяемым ресурсом (индекс)
                lck.unlock();

                seqModSearchInFullRangeByOnePrime(begin, n, prime, fullRange);
            }
        };

        thread t { handler };
        threads.push_back(move(t));
    }

    for (auto &t : threads)
    {
        t.join();
    }

    // Результат - массив с простыми числами из диапазона от (begin, n].
    SecondStepResult prime;

    for (size_t i = 0; i < diff; ++i)
    {
        if (!fullRange[i])
        {
            prime.push_back(i + (begin + 1));
        }
    }

    return prime;
}

int main(int argc, char *argv[])
{
    cout << thread::hardware_concurrency() << endl;

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
    const auto seqDecompositionByDataRes = timeBenchmark<SecondStepResult>(seqModSearch, sqrtN, n, ref(firstStepRes.second));
    cout << "> Seq decomposition by data: " << seqDecompositionByDataRes.first << " ms" << endl;
    writeToFile("SeqDecompositionByData.txt", seqDecompositionByDataRes.second);

    // Параллельная декомпозиция по данным: threadCount потоков
    const auto doParDecompositionByData = [&](uint8_t threadCount)
    {
        const auto parDecompositionByDataRes = timeBenchmark<SecondStepResult>(parDecompositionByData, sqrtN, n, ref(firstStepRes.second), threadCount);
        cout << "> Par decomposition by data [" + to_string(threadCount) + " threads]: " << parDecompositionByDataRes.first << " ms" << endl;
        writeToFile("ParDecompositionByData_" + to_string(threadCount) + "t.txt", parDecompositionByDataRes.second);
    };

    doParDecompositionByData(2);
    doParDecompositionByData(4);
    doParDecompositionByData(8);

    // Последовательная декомпозиция набора простых чисел
    const auto seqDecompositionByBasePrimeRes = timeBenchmark<SecondStepResult>(seqDecompositionByBasePrime, sqrtN, n, ref(firstStepRes.second));
    cout << "> Seq decomposition by base prime set: " << seqDecompositionByBasePrimeRes.first << " ms" << endl;
    writeToFile("SeqDecompositionByBasePrime.txt", seqDecompositionByBasePrimeRes.second);

    // Параллельная декомпозиция набора простых чисел: threadCount потоков
    const auto doParDecompositionByBasePrime = [n, sqrtN, &firstStepRes](uint8_t threadCount)
    {
        const auto parDecompositionByBasePrimeRes = timeBenchmark<SecondStepResult>(parDecompositionByBasePrime, sqrtN, n, ref(firstStepRes.second), threadCount);
        cout << "> Par decomposition by base prime set [" + to_string(threadCount) + " threads]: " << parDecompositionByBasePrimeRes.first << " ms" << endl;
        writeToFile("ParDecompositionByBasePrime_" + to_string(threadCount) + "t.txt", parDecompositionByBasePrimeRes.second);
    };

    doParDecompositionByBasePrime(2);
    doParDecompositionByBasePrime(4);
    doParDecompositionByBasePrime(8);

    // С применением Thread Pool
    const auto doParThreadPool = [n, sqrtN, &firstStepRes]()
    {
        const auto parThreadPoolRes = timeBenchmark<SecondStepResult>(parThreadPool, sqrtN, n, ref(firstStepRes.second), uint8_t(thread::hardware_concurrency()));
        cout << "> Par thread pool [" << thread::hardware_concurrency() << " threads] [" + to_string(firstStepRes.second.size()) + " tasks]: " << parThreadPoolRes.first << " ms" << endl;
        writeToFile("ParThreadPool.txt", parThreadPoolRes.second);
    };

    // Пул потоков
    doParThreadPool();

    // С последовательным перебором простых чисел
    const auto doParPrimeEnumeration = [n, sqrtN, &firstStepRes](uint8_t threadCount)
    {
        const auto parPrimeEnumerationRes = timeBenchmark<SecondStepResult>(parPrimeEnumeration, sqrtN, n, ref(firstStepRes.second), threadCount);
        cout << "> Par prime enumeration [" << int(threadCount) << " threads]: " << parPrimeEnumerationRes.first << " ms" << endl;
        writeToFile("ParPrimeEnumeration_" + to_string(threadCount) + "t.txt", parPrimeEnumerationRes.second);
    };

    doParPrimeEnumeration(2);
    doParPrimeEnumeration(4);
    doParPrimeEnumeration(8);

    system("pause");
    return 0;
}
/** @} */
