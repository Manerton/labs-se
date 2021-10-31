/** \defgroup main Главный модуль программы
    @{
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <thread>
#include <sstream>
#include <fstream>
#include <list>
#include <mutex>

#define debug

using namespace std;
using namespace chrono;

/// Миллисекунды в формате дробного числа.
using d_milliseconds = duration<double, milliseconds::period>;

using Message = std::string;
using MessageList = std::list<Message>;

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

/**
 * @brief Запись данных в файл.
 * @param filename Название файла.
 * @param data Контейнер с данными.
 */
template <typename T>
void writeToFile(const string& filename, const T& data)
{
    ofstream f(filename);

    if (f)
    {
        for (const auto& c: data)
        {
            f << c << " ";
        }
    }
}

/**
 * @brief Взаимодействие читателей и писателей без синхронизации.
 * @details Ожидамое поведение: функция никогда не завершит свою работу, поскольку в потоках происходит вечный цикл.
 *
 * Это происходит из-за того, что переменные-флаги агрессивно оптимизируются компилятором так, что изменения флага в одном потоке не видно в другом.
 * Т.е переменные-флаги (например булева bEmpty) непотокобезопасны.
 *
 * В качестве костыля, чтобы получить результат с потеренными и дублированными сообщениями в следствии отсутствия синхронизации,
 * можно отключить оптимизацию для флагов с помощью volatile, или же использовать atomic_bool.
 *
 * @param messageCount Общее количество сообщений.
 * @param messageLength Длина одного сообщения.
 * @param threadCount Количество потоков.
 * @return Список сообщений.
 */
MessageList withoutSync(const size_t messageCount,
                        const size_t messageLength,
                        const uint8_t threadCount
                        )
{
    mutex m;

    // буфер для сообщения
    Message buffer;

    // буфер чист?
    volatile bool bEmpty = true;

    // индекс текущего сообщения
    size_t messageIndex = 0;

    // сигнал о завершении работы писателей
    volatile bool finish = false;

    // список сообщений
    MessageList messages;

    const auto readerWork = [&]()
    {
        while (!finish)
        {
            if (!bEmpty)
            {
                messages.push_back(buffer);
                bEmpty = true;
            }
        }
    };

    const auto writerWork = [&](char threadId)
    {
        while (messageIndex < messageCount)
        {
            if (bEmpty)
            {
                Message newMessage = string(messageLength, threadId) + to_string(messageIndex++);
                buffer = newMessage;
                bEmpty = false;
            }
        }
    };

    // Запускаем читателей и писателей
    list<thread> writers;
    for (uint8_t i = 0; i < threadCount; ++i)
    {
        uint8_t threadId = 'A' + i;
        thread t { writerWork, threadId };
        writers.push_back(move(t));
    }

    list<thread> readers;
    for (size_t i = 0; i < threadCount; ++i)
    {
        thread t { readerWork };
        readers.push_back(move(t));
    }

    // Ожидаем завершения работы писателей
    for (auto &t : writers)
    {
        t.join();
    }

    // Сигнал о завершении работы писателей
    finish = true;

    // Ожидаем завершения работы для читателей
    for (auto &t : readers)
    {
        t.join();
    }

    return messages;
}

int main(int argc, char *argv[])
{
    constexpr size_t default_messageCount = 10;
    constexpr size_t default_messageLength = 10;

    auto messageCount = default_messageCount;
    auto messageLength = default_messageLength;

    if (argc >= 3)
    {
        messageCount = stoull(argv[1]);
        messageLength = stoull(argv[2]);
    }

    // Без средств синхронизации: threadCount потоков
    const auto doWithoutSync = [&](uint8_t threadCount)
    {
        const auto withoutSyncRes = timeBenchmark<MessageList>(withoutSync, messageCount, messageLength, threadCount);
        cout << "> without sync [" + to_string(threadCount) + " threads]: " << withoutSyncRes.first << " ms" << endl;
        writeToFile("withoutSync_" + to_string(threadCount) + "t.txt", withoutSyncRes.second);
    };

    doWithoutSync(2);
    doWithoutSync(4);
    doWithoutSync(8);

    system("pause");
    return 0;
}
/** @} */
