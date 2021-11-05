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

#include <boost/interprocess/sync/interprocess_semaphore.hpp>

#include "../utils.hpp"

using namespace std;

/// Сообщение.
using Message = std::string;
using MessageList = std::list<Message>;
using MessageVector = std::vector<Message>;

using Semaphore = boost::interprocess::interprocess_semaphore;
using SemaphorePtr = unique_ptr<Semaphore>;

using PriorityVector = std::vector<uint8_t>;

const static auto WrongIndex = numeric_limits<size_t>::max();

/// Длина сообщений
const static size_t messageLength = 10;

static deque<atomic<bool>> evReadyToRead;
static vector<SemaphorePtr> evStartReading;

static deque<atomic<bool>> evReadyToWrite;
static vector<SemaphorePtr> evStartWriting;

/**
 * Работа читателя.
 * @param iReader Номер потока-читателя.
 * @param buffer Кольцевой буфер.
 * @param readIndexCopy Массив с индексами буферных ячеек для читателей.
 * @param bFull Буфер заполнен?
 * @param finish Работа завершена?
 * @return
 */
MessageList readerThread(const size_t iReader,
                         const vector<Message>& buffer,
                         vector<size_t> &readIndexCopy,
                         atomic_bool &bFull,
                         const atomic_bool &finish
                         )
{
    MessageList localMessages;

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

        const size_t k = readIndexCopy[iReader];
        localMessages.push_back(buffer[k]);
        bFull = false;

        readIndexCopy[iReader] = WrongIndex;
    }

    return localMessages;
}

/**
 * Работа писателя.
 * @param iWriter Номер потока-писателя.
 * @param messageCount Количество сообщений писателя.
 * @param buffer Кольцевой буфер.
 * @param writeIndexCopy Массив с индексами буферных ячеек для писателей.
 * @param bEmpty Буфер пуст?
 */
void writerThread(const size_t iWriter,
                  const size_t messageCount,
                  vector<Message> &buffer,
                  vector<size_t> &writeIndexCopy,
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

    // Рабочий цикл записи
    while (thisMessageIndex < messageCount)
    {
        // Сигнализируем о готовности менеджеру
        evReadyToWrite[iWriter] = true;

        // Ждем сигнала от менеджера
        evStartWriting[iWriter]->wait();

        // Разрешена запись по текущему индексу
        const size_t k = writeIndexCopy[iWriter];
        buffer[k] = messages[thisMessageIndex++];
        bEmpty = false;

        writeIndexCopy[iWriter] = WrongIndex;
    }
}

size_t getBufferIndex(const size_t i, const size_t bufferSize)
{
    const size_t newIndex = (i + 1) % bufferSize;
    return newIndex;
}

/// Получение готового писателя с учетом приоритетов
size_t getWriter(const size_t nWriter, const PriorityVector& writerPriority)
{
    list<size_t> ready;
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
        return writerPriority[i] >= writerPriority[j];
    });

    return ready.front();
}

/// Получение готового читателя с учетом приоритетов
size_t getReader(const size_t nReader, const PriorityVector& readerPriority)
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

bool allWriterThreadsCompleted(const vector<atomic_bool>& v)
{
    return std::all_of(v.begin(), v.end(), [](const bool completed){ return completed; });
}

/**
 * Работа менеджера.
 * @param nReaders Количество читателей.
 * @param nWriters Количество писателей.
 * @param bufferSize Размер буфера.
 * @param messageCount Количество сообщений для одного писателя.
 * @return
 */
MessageList managerWork(const size_t nReaders,
                        const size_t nWriters,
                        const size_t bufferSize,
                        const size_t messageCount
                        )
{
    // Кольцевой буфер
    vector<Message> buffer(bufferSize);

    // Буфер заполнен?
    atomic<bool> bFull = false;

    // Буфер пуст?
    atomic<bool> bEmpty = true;

    // Конец работы
    atomic<bool> finish = false;

    // --- Подготовка читателей ---
    evReadyToRead.resize(nReaders);
    evStartReading.resize(nReaders);
    PriorityVector readerPriority(nReaders, 1);

    // Массив с индексами для читателей
    vector<size_t> readIndexCopy(nReaders, WrongIndex);
    vector<thread> tReaders(nReaders);

    // Список со списками сообщений от каждого потока
    list<MessageList> threadMessagesList;

    // Запуск читателей
    for (size_t i = 0; i < nReaders; ++i)
    {
        evReadyToRead[i] = false;
        evStartReading[i] = make_unique<Semaphore>(0);

        size_t i_copy = i;

        auto threadHandler = [i_copy, &buffer, &readIndexCopy, &bFull, &finish, &threadMessagesList]()
        {
            const MessageList localMessages = readerThread(i_copy, buffer, readIndexCopy, bFull, finish);
            threadMessagesList.emplace_back(localMessages);
        };

        thread t { threadHandler };
        tReaders[i] = move(t);
    }
    // ----------------------------

    // --- Подготовка писателей ---
    evReadyToWrite.resize(nWriters);
    evStartWriting.resize(nWriters);
    PriorityVector writerPriority(nWriters, 1);

    // Массив с индексами для писателей
    vector<size_t> writeIndexCopy(nWriters, WrongIndex);

    vector<thread> tWriters(nWriters);
    vector<atomic_bool> tWritersCompleted(nWriters);

    // Запуск писателей
    for (size_t i = 0; i < nWriters; ++i)
    {
        evReadyToWrite[i] = false;
        evStartWriting[i] = make_unique<Semaphore>(0);
        tWritersCompleted[i] = false;

        // TODO: Приоритеты перенести в функцию main
        if (i == 1)
        {
            writerPriority[i] = 4;
        }

        size_t i_copy = i;

        auto threadHandler = [i_copy, messageCount, &buffer, &writeIndexCopy, &bEmpty, &tWritersCompleted]()
        {
            writerThread(i_copy, messageCount, buffer, writeIndexCopy, bEmpty);
            tWritersCompleted[i_copy] = true;
        };

        thread t { threadHandler };
        t.detach();
        tWriters[i] = move(t);
    }
    // ----------------------------

    atomic<size_t> iW = 0;
    atomic<size_t> iR = 0;

    // Рабочий цикл
    while (!finish)
    {
        // Если в буфере есть свободные ячейки
        // пытаемся обработать готовых писателей
        if (!bFull && !contains(readIndexCopy, iW))
        {
            // Устанавливаем писателя, которому разрешаем работать
            size_t iWriter = getWriter(nWriters, writerPriority);

            if (iWriter != WrongIndex)
            {
                evReadyToWrite[iWriter] = false;

                // Сохраняем индекс для записи
                writeIndexCopy[iWriter] = iW;

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
            size_t iReader = getReader(nReaders, readerPriority);

            if (iReader != WrongIndex)
            {
                // Отмечаем, что теперь поток занят
                evReadyToRead[iReader] = false;

                // Сохраняем индекс для чтения
                readIndexCopy[iReader] = iR;

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

    cout << readedMessages.size() << endl;

    return readedMessages;
}

int main(int argc, char *argv[])
{
    // Дефолтные значения
    size_t default_bufferSize = 10;
    size_t default_messageCount = 400;

    // Размер буфера
    auto bufferSize = default_bufferSize;

    // Количество сообщений для одного писателя
    auto messageCount = default_messageCount;

    if (argc >= 3)
    {
        messageCount = stoull(argv[1]);
        bufferSize = stoull(argv[2]);
    }

    const auto doManagerSync = [&](const uint8_t _threadCount, const size_t _messageCount)
    {
        const auto withManagerSyncRes = timeBenchmark<MessageList>(managerWork, _threadCount, _threadCount, bufferSize, _messageCount);
        cout << "> Manager Sync [" + to_string(_threadCount) + " threads]: " << withManagerSyncRes.first << " ms" << endl;

        auto messages = withManagerSyncRes.second;
        messages.sort();

        writeToFile("managerSync_" + to_string(_threadCount) + "t.txt", messages);
    };

    doManagerSync(4, (messageCount/4));

    return 0;
}
/** @} */
