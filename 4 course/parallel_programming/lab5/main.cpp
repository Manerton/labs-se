/** \defgroup main Главный модуль программы
    @{
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>
#include <fstream>
#include <list>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <deque>
#include <random>

#include <boost/interprocess/sync/interprocess_semaphore.hpp>

#include "../utils.hpp"

using namespace std;

/// Тип для сообщения.
using Message = std::string;

/// Список сообщений.
using MessageList = std::list<Message>;

/// Массив сообщений.
using MessageVector = std::vector<Message>;

/// Boost Семафор.
using Semaphore = boost::interprocess::interprocess_semaphore;

/// Указатель на семафор.
using SemaphorePtr = unique_ptr<Semaphore>;

/// Массив с приоритетами для потоков.
using PriorityVector = std::vector<uint8_t>;

/// Индекс-индикатор.
const static auto WrongIndex = numeric_limits<size_t>::max();

static std::mt19937 mt_rand(uint32_t(time(nullptr)));
static std::uniform_int_distribution<> dist(0, 1);

/// Cтатистика работы программы
struct Stats
{
    vector<size_t> readedCount;
    vector<size_t> writtenCount;
    vector<double> readersTimes;
    vector<double> writersTimes;

    Stats(const size_t nReaders, const size_t nWriters)
    {
        readedCount.resize(nReaders, 0);
        readersTimes.resize(nReaders, 0);

        writtenCount.resize(nWriters, 0);
        writersTimes.resize(nWriters, 0);
    }
};

/**
 * Работа читателя.
 * @param iReader Номер потока-читателя.
 * @param buffer Кольцевой буфер.
 * @param readIndexCopy Массив с индексами буферных ячеек для читателей.
 * @param evReadyToRead Сигнал от читателя о готовности к работе.
 * @param evStartReading Сигнал от менеджера о начале работы.
 * @param bFull Буфер заполнен?
 * @param finish Работа завершена?
 * @return Пара значений - затраченное время и список прочитанных сообщений.
 */
pair<double, MessageList> readerThread(const size_t iReader,
                                       const vector<Message>& buffer,
                                       vector<atomic_size_t> &readIndexCopy,
                                       vector<atomic_bool> &evReadyToRead,
                                       vector<SemaphorePtr> &evStartReading,
                                       atomic_bool &bFull,
                                       const atomic_bool &finish
                                       )
{
    MessageList localMessages;

    // начинаем считать время
    auto start = std::chrono::steady_clock::now();

    // Рабочий цикл чтения
    while (!finish)
    {
        // Сигнализируем о готовности менеджеру
        evReadyToRead[iReader] = true;

        // Ждем сигнала от менеджера
        evStartReading[iReader]->wait();

        // Проверяем статус завершения работы
        if (finish && evReadyToRead[iReader])
        {
            break;
        }

        const size_t k = readIndexCopy[iReader].load();
        localMessages.push_back(buffer[k]);
        bFull = false;

        readIndexCopy[iReader].store(WrongIndex);
    }

    // заканчиваем считать время
    auto end = std::chrono::steady_clock::now();

    auto time = std::chrono::duration_cast<d_milliseconds>(end-start).count();

    return make_pair(time, localMessages);
}

/**
 * Работа писателя.
 * @param iWriter Номер потока-писателя.
 * @param messageCount Количество сообщений писателя.
 * @param messageLength Длина сообщений.
 * @param buffer Кольцевой буфер.
 * @param writeIndexCopy Массив с индексами буферных ячеек для писателей.
 * @param evReadyToWrite Сигнал от писателя о готовности к работе.
 * @param evStartWriting Сигнал от менеджера о начале работы.
 * @param bEmpty Буфер пуст?
 * @return Пара значений - затраченное время и количество записанных элементов.
 */
pair<double, size_t> writerThread(const size_t iWriter,
                                  const size_t messageCount,
                                  const size_t messageLength,
                                  vector<Message> &buffer,
                                  vector<atomic_size_t> &writeIndexCopy,
                                  vector<atomic_bool> &evReadyToWrite,
                                  vector<SemaphorePtr> &evStartWriting,
                                  atomic_bool &bEmpty
                                  )
{
    // Буква потока
    uint8_t threadId = 'A' + uint8_t(iWriter);

    // Кол-во сообщений этого потока
    size_t thisMessageIndex = 0;

    MessageVector messages(messageCount);

    const size_t messageCountStrLen = to_string(messageCount).length();

    for (size_t i = 0; i < messageCount; ++i)
    {
        auto number = to_string(i);
        messages[i] = string(messageLength, char(threadId)) + string(messageCountStrLen - number.length(), '0') + number;
    }

    // начинаем считать время
    auto start = std::chrono::steady_clock::now();

    // Рабочий цикл записи
    while (thisMessageIndex < messageCount)
    {
        // Сигнализируем о готовности менеджеру
        evReadyToWrite[iWriter] = true;

        // Ждем сигнала от менеджера
        evStartWriting[iWriter]->wait();

        // Разрешена запись по текущему индексу
        const size_t k = writeIndexCopy[iWriter].load();
        buffer[k] = messages[thisMessageIndex++];
        bEmpty = false;

        writeIndexCopy[iWriter].store(WrongIndex);
    }

    // заканчиваем считать время
    auto end = std::chrono::steady_clock::now();

    auto time = std::chrono::duration_cast<d_milliseconds>(end-start).count();

    return make_pair(time, thisMessageIndex);
}

/// Возвращает следующий индекс в кольцевом буфере
size_t getBufferIndex(const size_t i, const size_t bufferSize)
{
    const size_t newIndex = (i + 1) % bufferSize;
    return newIndex;
}

/**
 * Получение готового к работе читателя с учетом приоритетов.
 * @param evReadyToRead Массив с сигналами готовности к работе читателей.
 * @param readerPriority Массив с приоритетами каждого читателя.
 * @return Индекс читателя или WrongIndex, если все читатели заняты.
 */
size_t getReader(const vector<atomic_bool> &evReadyToRead,
                 const PriorityVector& readerPriority
                 )
{
    list<size_t> ready;
    const size_t nReader = evReadyToRead.size();
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
        if (readerPriority[i] == readerPriority[j])
        {
            return bool(dist(mt_rand));
        }

        return readerPriority[i] > readerPriority[j];
    });

    return ready.front();
}

/**
 * Получение готового к работе писателя с учетом приоритетов.
 * @param evReadyToWrite Массив с сигналами готовности к работе писателей.
 * @param writerPriority Массив с приоритетами каждого писателя.
 * @return Индекс писателя или WrongIndex, если все писатели заняты.
 */
size_t getWriter(const vector<atomic_bool> &evReadyToWrite,
                 const PriorityVector& writerPriority
                 )
{
    list<size_t> ready;
    const size_t nWriter = evReadyToWrite.size();

    for (size_t i = 0; i < nWriter; ++i)
    {
        if (evReadyToWrite[i])
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
        if (writerPriority[i] == writerPriority[j])
        {
            return bool(dist(mt_rand));
        }

        return writerPriority[i] > writerPriority[j];
    });

    return ready.front();
}

/// Все писатели закончили работу?
bool allWriterThreadsCompleted(const vector<atomic_bool>& v)
{
    return std::all_of(v.begin(), v.end(),
                       [](bool completed){ return completed; }
    );
}

/**
 * Работа менеджера.
 * @param nReaders Количество читателей.
 * @param readerPriority Приоритеты читателей.
 * @param nWriters Количество писателей.
 * @param writerPriority Приоритеты писателей.
 * @param bufferSize Размер буфера.
 * @param messageCount Количество сообщений для одного писателя.
 * @param messageLength Длина одного сообщения.
 * @return Список сообщений и статистика.
 */
pair<MessageList, Stats> managerWork(const size_t nReaders,
                                     const PriorityVector &readerPriority,
                                     const size_t nWriters,
                                     const PriorityVector &writerPriority,
                                     const size_t bufferSize,
                                     const size_t messageCount,
                                     const size_t messageLength
                                     )
{
    Stats stats(nReaders, nWriters);

    // Кольцевой буфер
    vector<Message> buffer(bufferSize);

    // Буфер заполнен?
    atomic<bool> bFull = false;

    // Буфер пуст?
    atomic<bool> bEmpty = true;

    // Конец работы
    atomic<bool> finish = false;

    // --- Подготовка читателей ---

    // Сигналы готовности к работе читателей.
    vector<atomic_bool> evReadyToRead(nReaders);

    // Семафоры-события начала чтения читателем.
    vector<SemaphorePtr> evStartReading(nReaders);

    // Массив с индексами для читателей.
    vector<atomic_size_t> readIndexCopy(nReaders);

    // Потоки-читатели.
    vector<thread> tReaders(nReaders);

    // Список со списками сообщений от каждого потока
    list<MessageList> threadMessagesList;

    // Запуск читателей
    for (size_t i = 0; i < nReaders; ++i)
    {
        evReadyToRead[i] = false;
        evStartReading[i] = make_unique<Semaphore>(0);
        readIndexCopy[i].store(WrongIndex);

        size_t i_copy = i;

        // Всё в лямбду передаем по ссылке, а i_copy по значению.
        auto threadHandler = [&, i_copy]()
        {
            const auto res = readerThread(i_copy,
                                          buffer, readIndexCopy,
                                          evReadyToRead, evStartReading,
                                          bFull, finish
                                          );

            threadMessagesList.emplace_back(res.second);

            // записываем статистику
            stats.readersTimes[i_copy] = res.first;
            stats.readedCount[i_copy] = res.second.size();
        };

        thread t { threadHandler };
        tReaders[i] = move(t);
    }
    // ----------------------------

    // --- Подготовка писателей ---

    // Сигналы готовности к работе писателей.
    vector<atomic_bool> evReadyToWrite(nWriters);

    // Семафоры-события начала записи писателем.
    vector<SemaphorePtr> evStartWriting(nWriters);

    // Массив с индексами для писателей.
    vector<atomic_size_t> writeIndexCopy(nWriters);

    // Потоки-писатели.
    vector<thread> tWriters(nWriters);

    // Сигналы о завершении писателей.
    vector<atomic_bool> tWritersCompleted(nWriters);

    // Запуск писателей
    for (size_t i = 0; i < nWriters; ++i)
    {
        evReadyToWrite[i] = false;
        evStartWriting[i] = make_unique<Semaphore>(0);
        writeIndexCopy[i].store(WrongIndex);
        tWritersCompleted[i] = false;

        size_t i_copy = i;

        // Всё по ссылке передаем в лямбду, i_copy и messageCount по значению
        auto threadHandler = [&, i_copy, messageCount]()
        {
            auto res = writerThread(i_copy,
                                    messageCount, messageLength,
                                    buffer, writeIndexCopy,
                                    evReadyToWrite, evStartWriting,
                                    bEmpty
                                    );

            // как только поток завершит работу, отметим это
            tWritersCompleted[i_copy] = true;

            // записываем статистику
            stats.writersTimes[i_copy] = res.first;
            stats.writtenCount[i_copy] = res.second;
        };

        thread t { threadHandler };
        t.detach();
        tWriters[i] = move(t);
    }
    // ----------------------------

    size_t iW = 0;
    size_t iR = 0;

    // Рабочий цикл
    while (!finish)
    {
        // Если в буфере есть свободные ячейки
        // пытаемся обработать готовых писателей
        if (!bFull && !contains(readIndexCopy, iW))
        {
            // Устанавливаем писателя, которому разрешаем работать
            const size_t iWriter = getWriter(evReadyToWrite, writerPriority);

            if (iWriter != WrongIndex)
            {
                evReadyToWrite[iWriter] = false;

                // Сохраняем индекс для записи
                writeIndexCopy[iWriter].store(iW);

                // Разрешаем писателю начать работу
                evStartWriting[iWriter]->post();

                iW = getBufferIndex(iW, bufferSize);

                if (iW == iR)
                {
                    bFull = true;
                }
            }
        }

        // Если буфер не пуст, пытаемся
        // обработать готовых писателей
        if (!bEmpty && !contains(writeIndexCopy, iR))
        {
            // Устанавливаем читателя, которому разрешаем работать
            const size_t iReader = getReader(evReadyToRead, readerPriority);

            if (iReader != WrongIndex)
            {
                // Отмечаем, что теперь поток занят
                evReadyToRead[iReader] = false;

                // Сохраняем индекс для чтения
                readIndexCopy[iReader].store(iR);

                // Разрешаем читателю начать работу
                evStartReading[iReader]->post();

                iR = getBufferIndex(iR, bufferSize);

                if (iR == iW)
                {
                    bEmpty = true;
                }

            }
        }

        // Если писатели закончили работу
        if (allWriterThreadsCompleted(tWritersCompleted) && bEmpty)
        {
            finish = true;
        }
    }

    for (auto& event : evStartReading)
    {
        event->post();
    }

    for (auto &t : tReaders)
    {
        t.join();
    }

    MessageList readedMessages;

    for (auto &l : threadMessagesList)
    {
        readedMessages.insert(readedMessages.end(), l.begin(), l.end());
    }

    return make_pair(readedMessages, stats);
}

int main(int argc, char *argv[])
{
    // Дефолтные значения
    const size_t default_bufferSize = 10;
    const size_t default_messageCount = 400;
    const size_t default_messageLength = 10;

    // Размер буфера
    auto bufferSize = default_bufferSize;

    // Количество сообщений для одного писателя
    auto messageCount = default_messageCount;

    // Длина сообщений
    auto messageLength = default_messageLength;

    if (argc >= 2)
    {
        bufferSize = stoull(argv[1]);
    }

    if (argc >= 3)
    {
        messageCount = stoull(argv[2]);
    }

    if (argc >= 4)
    {
        messageLength = stoull(argv[3]);
    }

    cout << "Size of the ring buffer: " << bufferSize << endl;

    const auto doManagerSync = [&](
            const size_t readerCount,
            const size_t writerCount,
            const size_t _messageCount,
            const PriorityVector &readerPriority,
            const PriorityVector &writerPriority
            )
    {
        cout << "> Start manager sync [" + to_string(readerCount) + " readers; " + to_string(writerCount) + " writers]" << endl;
        cout << "Message length: " << messageLength << endl;
        cout << "Message count for one writer: " << _messageCount << endl;
        cout << "Total message count: " << _messageCount * writerCount << endl;

        for (size_t i = 0; i < readerCount; ++i)
        {
            cout << "Reader thread #" << to_string(i) << " priority: " << uint32_t(readerPriority[i]) << endl;
        }

        for (size_t i = 0; i < writerCount; ++i)
        {
            cout << "Writer thread #" << to_string(i) << " priority: " << uint32_t(writerPriority[i]) << endl;
        }

        const auto res = timeBenchmark<pair<MessageList, Stats>>(managerWork,
                                                                 readerCount, readerPriority,
                                                                 writerCount, writerPriority,
                                                                 bufferSize, _messageCount, messageLength
                                                                 );

        cout << "Total time: " << res.first << " ms" << endl;

        auto stats = res.second.second;

        for (size_t i = 0; i < writerCount; ++i)
        {
            cout << "Writer #" << to_string(i) << " recorded: " << stats.writtenCount[i] << " messages in " << stats.writersTimes[i] << " ms" << endl;
        }

        for (size_t i = 0; i < readerCount; ++i)
        {
            cout << "Reader #" << to_string(i) << " readed: " << stats.readedCount[i] << " messages in " << stats.readersTimes[i] << " ms" << endl;
        }

        auto messages = res.second.first;

        cout << "Total readed: " << messages.size() << endl;

        messages.sort();

        writeToFile("managerSync_" + to_string(readerCount) + to_string(writerCount) + "t.txt", messages);
    };

    doManagerSync(1, 1, (messageCount), {1, 1, 1, 1}, {1, 1, 1, 1});
    doManagerSync(1, 4, (messageCount/4), {1, 1, 1, 1}, {1, 1, 1, 1});
    doManagerSync(2, 2, (messageCount/2), {1, 1, 1, 1}, {1, 1, 1, 1});
    doManagerSync(4, 1, (messageCount), {1, 1, 1, 1}, {1, 1, 1, 1});

    doManagerSync(4, 4, (messageCount/4), {1, 1, 1, 1}, {1, 1, 1, 1});
    doManagerSync(4, 4, (messageCount/4), {1, 2, 3, 4}, {1, 2, 3, 4});
    doManagerSync(4, 4, (messageCount/4), {1, 2, 1, 1}, {1, 1, 2, 1});


    return 0;
}
/** @} */
