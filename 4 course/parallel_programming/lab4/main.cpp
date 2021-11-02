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
#include <atomic>
#include <condition_variable>

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
void writeToFile(const string& filename, const T& data, const string& delim = "\r\n")
{
    ofstream f(filename);

    if (f)
    {
        for (const auto& c: data)
        {
            f << c << delim;
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
    // буфер для сообщения
    Message buffer;

    // буфер чист?
    atomic<bool> bEmpty = true;

    // индекс текущего сообщения
    atomic<size_t> messageIndex = 0;

    // сигнал о завершении работы писателей
    atomic<bool> finish = false;

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
            if (bEmpty && messageIndex < messageCount)
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

/**
 * @brief Взаимодействие читателей и писателей с синхронизацией с помощью мьютекса (блокировка, lock).
 * @details Переменные-флаги bEmpty, finish и счётчик messageIndex должны быть атомарными (atomic),
 * поскольку они явно не защищены блокировкой (мьютексом)
 * и к ним осуществляется параллельный одновременный доступ из разных потоков,
 * то есть возникает гонка данных.
 *
 * Буфер buffer и сообщения messages не требуют атомарности (их и нельзя сделать атомарными), поскольку явно защищаются мьютексом.
 *
 * @param messageCount Общее количество сообщений.
 * @param messageLength Длина одного сообщения.
 * @param threadCount Количество потоков.
 * @return Список сообщений.
 */
MessageList withMutexSync(const size_t messageCount,
                          const size_t messageLength,
                          const uint8_t threadCount
                          )
{
    mutex m;

    // буфер для сообщения
    Message buffer;

    // буфер чист?
    atomic<bool> bEmpty = true;

    // индекс текущего сообщения
    atomic<size_t> messageIndex = 0;

    // сигнал о завершении работы писателей
    atomic<bool> finish = false;

    // список сообщений
    MessageList messages;

    const auto readerWork = [&]()
    {
        while (!finish)
        {
            if (!bEmpty)
            {
                scoped_lock readLock {m};
                if (!bEmpty)
                {
                    messages.push_back(buffer);
                    bEmpty = true;
                }
            }
        }
    };

    const auto writerWork = [&](char threadId)
    {
        while (messageIndex < messageCount)
        {
            if (bEmpty)
            {
                scoped_lock writeLock {m};
                if (bEmpty && messageIndex < messageCount)
                {
                    Message newMessage = string(messageLength, threadId) + to_string(messageIndex++);
                    buffer = newMessage;
                    bEmpty = false;
                }
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

/**
 * @brief Взаимодействие читателей и писателей с синхронизацией с помощью событий (conditional variable).
 * @param messageCount Общее количество сообщений.
 * @param messageLength Длина одного сообщения.
 * @param threadCount Количество потоков.
 * @return Список сообщений.
 */
MessageList withEventSync(const size_t messageCount,
                          const size_t messageLength,
                          const uint8_t threadCount
                          )
{
    mutex m;

    // События пустого и заполненного буфера
    condition_variable evFull;
    condition_variable evEmpty;

    // Буфер для сообщения
    Message buffer;

    // Буфер чист?
    bool bEmpty = true;

    // Индекс текущего сообщения
    atomic<size_t> messageIndex = 0;

    // Сигнал о завершении работы писателей
    atomic<bool> finish = false;

    // Список сообщений
    MessageList messages;

    const auto readerWork = [&]()
    {
        while (true)
        {
            // Захватываем мьютекс
            unique_lock lck {m};

            // Освобождаем мьютекс и ожидаем событие
            // пробуждаемся по событию,
            // при пробуждении проверяем флаг (на случай ложных пробуждений),
            // повторно захватываем мьютекс при пробуждении и работаем с критической секцией.
            evFull.wait(lck, [&]{ return !bEmpty || finish; });

            // Сигнал о завершении работы
            if (finish)
            {
                break;
            }

            messages.push_back(buffer);
            bEmpty = true;

            // Сообщаем одному из писателей, что буфер опустошен.
            evEmpty.notify_one();
        }
    };

    const auto writerWork = [&](char threadId)
    {
        while (messageIndex < messageCount)
        {
            unique_lock lck {m};

            evEmpty.wait(lck, [&]{ return bEmpty; });

            // Несколько потоков могли успеть попасть в этот участок кода
            // когда messageIndex был меньше чем messageCount.

            // Один из таких потоков захватил мьютекс, увеличил индекс сообщения, и когда он освободил мьютекс
            // и другой поток получил управление, то индекс messageIndex оказался уже больше или равен messageCount.
            if (messageIndex >= messageCount)
            {
                break;
            }

            Message newMessage = string(messageLength, threadId) + to_string(messageIndex++);
            buffer = newMessage;
            bEmpty = false;

            // Сообщаем одному из читателей, что буфер заполнен.
            evFull.notify_one();
        }

       // Остальные потоки, индекс сообщения которых вышел за пределы в следствие работы других потоков
       // до сих пор ожидают сигнала от читателей.
       // Поэтому "разбудим" оставшиеся "спящие" потоки с неправильными индексами самостоятельно.
       if (messageIndex >= messageCount)
       {
           evEmpty.notify_all();
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
    evFull.notify_all();

    // Ожидаем завершения работы для читателей
    for (auto &t : readers)
    {
        t.join();
    }

    return messages;
}

/**
 * @brief Взаимодействие читателей и писателей с синхронизацией с помощью атомарных переменных и операций.
 *
 * Буфер и список сообщений невозможно сделать атомарными, так как тип буфера - string.
 * Соответственно в данной функции атомарная переменная usingResource используется для блокировки критической секции
 * (флаг барьер, самодельный lock).
 *
 * @param messageCount Общее количество сообщений.
 * @param messageLength Длина одного сообщения.
 * @param threadCount Количество потоков.
 * @return Список сообщений.
 */
MessageList withAtomicSync(const size_t messageCount,
                           const size_t messageLength,
                           const uint8_t threadCount
                           )
{
    // занят ли потоком буфер?
    atomic<bool> usingResource = false;

    // буфер для сообщения
    Message buffer;

    // буфер чист?
    atomic<bool> bEmpty = true;

    // индекс текущего сообщения
    atomic<size_t> messageIndex = 0;

    // сигнал о завершении работы писателей
    atomic<bool> finish = false;

    // список сообщений
    MessageList messages;

    const auto readerWork = [&]()
    {
        while (!finish)
        {
            if (!bEmpty)
            {
                // занимаем ресурс,
                // и если он был не занят до этого,
                // то работаем с ним
                if (!usingResource.exchange(true))
                {
                    if (!bEmpty)
                    {
                        messages.push_back(buffer);
                        bEmpty = true;
                    }
                    // освобождаем ресурс
                    usingResource.store(false);
                }
            }
        }
    };

    const auto writerWork = [&](char threadId)
    {
        while (messageIndex < messageCount)
        {
            if (bEmpty)
            {
                if (!usingResource.exchange(true))
                {
                    if (bEmpty && messageIndex < messageCount)
                    {
                        Message newMessage = string(messageLength, threadId) + to_string(messageIndex++);
                        buffer = newMessage;
                        bEmpty = false;
                    }
                    usingResource.store(false);
                }
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
    constexpr size_t default_messageCount = 100;
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
        const auto withoutSyncRes = timeBenchmark<MessageList>(withoutSync, default_messageCount, default_messageLength, threadCount);
        cout << "> without sync [" + to_string(threadCount) + " threads]: " << withoutSyncRes.first << " ms" << endl;
        writeToFile("withoutSync_" + to_string(threadCount) + "t.txt", withoutSyncRes.second);
    };

    doWithoutSync(1);
    doWithoutSync(2);
    doWithoutSync(4);

    // Синхронизация мьютексом (блокировка): threadCount потоков
    const auto doWithMutexSync = [&](uint8_t threadCount)
    {
        const auto withMutexSyncRes = timeBenchmark<MessageList>(withMutexSync, messageCount, messageLength, threadCount);
        cout << "> with Mutex Sync [" + to_string(threadCount) + " threads]: " << withMutexSyncRes.first << " ms" << endl;
        writeToFile("withMutexSync_" + to_string(threadCount) + "t.txt", withMutexSyncRes.second);
    };

    doWithMutexSync(1);
    doWithMutexSync(2);
    doWithMutexSync(4);

    // Синхронизация событиями (condition variables): threadCount потоков
    const auto doWithEventSync = [&](uint8_t threadCount)
    {
        const auto withEventSyncRes = timeBenchmark<MessageList>(withEventSync, messageCount, messageLength, threadCount);
        cout << "> with Event Sync [" + to_string(threadCount) + " threads]: " << withEventSyncRes.first << " ms" << endl;
        writeToFile("withEventSync_" + to_string(threadCount) + "t.txt", withEventSyncRes.second);
    };

    doWithEventSync(1);
    doWithEventSync(2);
    doWithEventSync(4);

    // Синхронизация атомарными операциями: threadCount потоков
    const auto doWithAtomicSync = [&](uint8_t threadCount)
    {
        const auto withAtomicSyncRes = timeBenchmark<MessageList>(withAtomicSync, messageCount, messageLength, threadCount);
        cout << "> with Atomic Sync [" + to_string(threadCount) + " threads]: " << withAtomicSyncRes.first << " ms" << endl;
        writeToFile("withAtomicSync_" + to_string(threadCount) + "t.txt", withAtomicSyncRes.second);
    };

    doWithAtomicSync(1);
    doWithAtomicSync(2);
    doWithAtomicSync(4);

    return 0;
}
/** @} */
