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
#include <deque>

#include <boost/interprocess/sync/interprocess_semaphore.hpp>

using namespace std;
using namespace chrono;

/// Миллисекунды в формате дробного числа.
using d_milliseconds = duration<double, milliseconds::period>;

using Message = std::string;
using MessageList = std::list<Message>;

using Semaphore = boost::interprocess::interprocess_semaphore;
using SemaphorePtr = unique_ptr<Semaphore>;

const static auto WrongIndex = numeric_limits<size_t>::max();

/// Вывести содержимое контейнера (где есть begin и end).
template <typename C>
string displayContainer(const C& container)
{
    typename C::const_iterator it;

    stringstream ss;

    for (it = container.begin(); it != container.end(); ++it)
    {
        ss << *it << "\n";
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

/// Массив с индексами для читателей
static vector<size_t> ReadIndexCopy;

/// Массив с индексами для писателей
static vector<size_t> WriteIndexCopy;

/// Кольцевой буфер
static vector<Message> buffer;

static deque<mutex> mBuffer;

/// Буфер заполнен?
static atomic<bool> bFull = false;

/// Буфер пуст?
static atomic<bool> bEmpty = true;

/// Конец работы писателей
static atomic<bool> finishWriters = false;

/// Конец работы читателей
static atomic<bool> finishReaders = false;

/// Количество сообщений
static size_t messageCount = 50;

/// Длина сообщений
static size_t messageLength = 10;

/// Номер текущего сообщения
static atomic<size_t> messageIndex = 0;

static deque<atomic<bool>> evReadyToRead;
static vector<SemaphorePtr> evStartReading;

static deque<atomic<bool>> evReadyToWrite;
static vector<SemaphorePtr> evStartWriting;

/// Приоритеты писателей
static vector<uint8_t> writerPriority;

/// Приоритеты читателей
static vector<uint8_t> readerPriority;

/// Потоки читатели
static vector<thread> tReaders;

/// Потоки писатели
static vector<thread> tWriters;

/// список сообщений
static MessageList readedMessages;

static atomic<uint64_t> writerCounter;
static atomic<uint64_t> readerCounter;
static atomic<uint64_t> readerAttemptCounter;

/**
 * @brief Работа потока-читателя.
 * @param iReader Индекс читателя.
 */
MessageList readerThread(const size_t iReader)
{
    MessageList localMessages;

    // Рабочий цикл чтения
    while (true)
    {
        // Сигнализируем о готовности менеджеру
        evReadyToRead[iReader] = true;

        ++readerAttemptCounter;

        // Ждем сигнала от менеджера
        evStartReading[iReader]->wait();

        // Проверяем статус завершения работы
        if (finishReaders)
        {
            break;
        }

        // Разрешено чтение по текущему индексу
        size_t k = ReadIndexCopy[iReader];

        scoped_lock readLck {mBuffer[k]};
        localMessages.push_back(buffer[k]);

        bFull = false;

        ++readerCounter;
    }
    return localMessages;
}

/**
 * @brief Работа потока-писателя.
 * @param iWriter Индекс писателя.
 */
void writerThread(const size_t iWriter)
{
    // Буква потока
    uint8_t threadId = 'A' + uint8_t(iWriter);

    // Кол-во сообщений этого потока
    size_t thisMessageIndex = 0;

    // Рабочий цикл записи
    while(true)
    {
        // Сигнализируем о готовности менеджеру
        evReadyToWrite[iWriter] = true;

        // Ждем сигнала от менеджера
        evStartWriting[iWriter]->wait();

        if (finishWriters)
        {
            break;
        }

        // Разрешена запись по текущему индексу
        size_t k = WriteIndexCopy[iWriter];

        Message newMessage = string(messageLength, char(threadId)) + to_string(thisMessageIndex++);

        scoped_lock writeLck {mBuffer[k]};
        buffer[k] = newMessage;

        bEmpty = false;

        ++messageIndex;
        ++writerCounter;
    }
}

size_t getBufferIndex(const size_t i)
{
    size_t newIndex = (i + 1) % buffer.size();
    return newIndex;
}

/// Получение готового писателя с учетом приоритетов
size_t getWriter(const size_t nWriter)
{
    list<size_t> ready;
    for (size_t i = 0; i < nWriter; ++i)
    {
        if (evReadyToWrite[i].load())
        {
            ready.push_back(i);
        }
    }

    if (ready.empty())
    {
        return WrongIndex;
    }

    ready.sort([&](size_t i, size_t j)
    {
        return writerPriority[i] >= writerPriority[j];
    });

    return ready.front();
}

/// Получение готового читателя с учетом приоритетов
size_t getReader(const size_t nReader)
{
    list<size_t> ready;
    for (size_t i = 0; i < nReader; ++i)
    {
        if (evReadyToRead[i])
        {
            ready.push_back(i);
        }
    }

    if (ready.empty())
    {
        return WrongIndex;
    }

    ready.sort([&](size_t i, size_t j)
    {
        return readerPriority[i] >= readerPriority[j];
    });

    return ready.front();
}

MessageList managerWork(const size_t nReaders, const size_t nWriters, const size_t bufferSize)
{
    list<MessageList> threadMessagesList;

    buffer.resize(bufferSize);
    mBuffer.resize(bufferSize);

    evReadyToRead.resize(nReaders);
    evStartReading.resize(nReaders);
    readerPriority.resize(nReaders, 1);
    ReadIndexCopy.resize(nReaders);
    tReaders.resize(nReaders);

    // Запуск читателей
    for (size_t i = 0; i < nReaders; ++i)
    {
        evReadyToRead[i] = false;
        evStartReading[i] = make_unique<Semaphore>(0);

        auto threadHandler = [i, &threadMessagesList]()
        {
            const MessageList localMessages = readerThread(i);
            threadMessagesList.emplace_back(localMessages);
        };

        thread t { threadHandler };
        tReaders[i] = move(t);
    }

    evReadyToWrite.resize(nWriters);
    evStartWriting.resize(nWriters);
    writerPriority.resize(nWriters, 1);
    WriteIndexCopy.resize(nWriters);
    tWriters.resize(nWriters);

    // Запуск писателей
    for (size_t i = 0; i < nWriters; ++i)
    {
        evReadyToWrite[i] = false;
        evStartWriting[i] = make_unique<Semaphore>(0);

        // TODO: Приоритеты перенести в функцию main
        if (i == 1)
        {
            writerPriority[i] = 4;
        }

        thread t { writerThread, i };
        tWriters[i] = move(t);
    }

    size_t iW = WrongIndex;
    size_t iR = WrongIndex;

    size_t iWriter = WrongIndex;
    size_t iReader = WrongIndex;

    // Рабочий цикл
    while (true)
    {
        // Если в буфере есть свободные ячейки
        // пытаемся обработать готовых писателей
        if (!bFull && (messageIndex < messageCount))
        {
            // Устанавливаем писателя, которому разрешаем работать
            iWriter = getWriter(nWriters);

            if (iWriter != WrongIndex)
            {
                if (getBufferIndex(iW) != iR)
                {
                    iW = getBufferIndex(iW);

                    // Сохраняем индекс для записи
                    WriteIndexCopy[iWriter] = iW;

                    evReadyToWrite[iWriter] = false;
                    // Разрешаем писателю начать работу
                    evStartWriting[iWriter]->post();
                }
                else
                {
                    bFull = true;
                    bEmpty = false;
                }
            }
        }

        // Если буфер не пуст, пытаемся
        // обработать готовых писателей
        if (!bEmpty)
        {
            // Устанавливаем читателя, которому разрешаем работать
            iReader = getReader(nReaders);

            if (iReader != WrongIndex)
            {
                if (getBufferIndex(iR) != iW)
                {
                    iR = getBufferIndex(iR);

                    // Сохраняем индекс для чтения
                    ReadIndexCopy[iReader] = iR;

                    evReadyToRead[iReader] = false;
                    // Разрешаем читателю начать работу
                    evStartReading[iReader]->post();
                }
                else
                {
                    bEmpty = true;
                    bFull = false;
                }
            }
        }

        // Если писатели закончили работу
        if (!finishWriters && (messageIndex >= messageCount))
        {
            finishWriters = true;
            for (auto& event : evStartWriting)
            {
                event->post();
            }

            for (auto &t : tWriters)
            {
                t.join();
            }
        }

        // Если читатели закончили работу
        if (finishWriters && bEmpty)
        {
            finishReaders = true;

            for (auto& event : evStartReading)
            {
                event->post();
            }

            for (auto &t : tReaders)
            {
                t.join();
            }

            break;
        }
    }

    for (auto &l : threadMessagesList)
    {
        readedMessages.insert(readedMessages.end(), l.begin(), l.end());
    }

    readedMessages.unique();
    cout << writerCounter << endl;
    cout << readedMessages.size() << endl;
    cout << readerCounter << endl;
    cout << readerAttemptCounter << endl;

    //cout << displayContainer(steps) << endl;

    return readedMessages;
}

int main(int argc, char *argv[])
{
    size_t default_bufferSize = 10;
    auto bufferSize = default_bufferSize;

    if (argc >= 3)
    {
        messageCount = stoull(argv[1]);
        bufferSize = stoull(argv[2]);
    }

    const auto doManagerSync = [&](uint8_t threadCount)
    {
        const auto withManagerSyncRes = timeBenchmark<MessageList>(managerWork, threadCount, threadCount, bufferSize);
        cout << "> Manager Sync [" + to_string(threadCount) + " threads]: " << withManagerSyncRes.first << " ms" << endl;
        writeToFile("managerSync_" + to_string(threadCount) + "t.txt", withManagerSyncRes.second);
    };

    doManagerSync(4);

    return 0;
}
/** @} */
