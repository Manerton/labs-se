#include <iostream>
#include <chrono>
#include <sstream>

using std::string;
using std::stringstream;
using std::pair;

/// Миллисекунды в формате дробного числа.
using d_milliseconds = std::chrono::duration<double, std::chrono::milliseconds::period>;

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

    ss << std::endl;

    return ss.str();
}

/**
 * @brief Бенчмарк.
 * @param func Выполняемая функция.
 * @param args Аргументы выполняемой функции.
 * @return Результат вычисления и время выполнения функции.
 */
template <typename duration, typename Result, typename Function, typename... Args>
pair<double, Result> timeBenchmark(const Function& func, const Args&... args)
{
    // начинаем считать время
    auto start = std::chrono::steady_clock::now();

    // что-то вычисляем
    Result calc = func(args...);

    // заканчиваем считать время
    auto end = std::chrono::steady_clock::now();

    auto time = std::chrono::duration_cast<duration>(end-start).count();

    return make_pair(time, calc);
}

/**
 * @brief Бенчмарк.
 * @param func Выполняемая функция.
 * @param args Аргументы выполняемой функции.
 * @return Результат вычисления и время выполнения функции.
 */
template <typename duration, typename Result, typename Function, typename Object, typename... Args>
pair<double, Result> timeBenchmarkForMemberFunc(const Function& func, const Object* obj, const Args&... args)
{
    // начинаем считать время
    auto start = std::chrono::steady_clock::now();

    // что-то вычисляем
    Result calc = (obj->*func)(args...);

    // заканчиваем считать время
    auto end = std::chrono::steady_clock::now();

    auto time = std::chrono::duration_cast<duration>(end-start).count();

    return make_pair(time, calc);
}

/**
 * @brief Запись данных в файл.
 * @param filename Название файла.
 * @param data Контейнер с данными.
 */
template <typename T>
void writeToFile(const string& filename, const T& data, const string& delim = "\r\n")
{
    std::ofstream f(filename);

    if (f)
    {
        for (const auto& c: data)
        {
            f << c << delim;
        }
    }
}

/**
 * @brief Имеется ли элемент в контейнере?
 * @param container Контейнер с данными.
 * @param value Искомый элемент.
 * @return Булево выражение - результат поиска.
 */
template <typename C, typename V>
bool contains(const C& container, const V& value)
{
    auto find_res = std::find(container.begin(), container.end(), value);
    return (find_res != container.end());
}


