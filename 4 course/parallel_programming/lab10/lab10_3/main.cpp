#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include <array>
#include <string>
#include <iostream>

#include <boost/interprocess/sync/named_semaphore.hpp>

using namespace std;
using namespace boost::interprocess;

enum class PipeStream
{
    readStream = 0,
    writeStream = 1
};

using Pipe = array<int, 2>;

void writeToPipe(const Pipe& pipe, const string& str)
{
    // Длина строки (которая например не больше чем 65к символов).
    auto strLen = uint16_t(str.size());

    // Пишем в pipe.
    auto pipeIndex = size_t(PipeStream::writeStream);

    // Сначала запишем размер строки.
    auto writtenSize = write(pipe[pipeIndex], &strLen, sizeof(strLen));

    if (writtenSize != sizeof(strLen))
    {
        // Если записалось меньшее количество байт, сообщаем об ошибке.
        cout << "Can't write length of string: " << str << endl;
        exit(-1);
    }

    // Теперь запишем саму строку.
    auto actualSize = write(pipe[pipeIndex], str.c_str(), strLen);

    if (actualSize != strLen)
    {
        // Если записалось меньшее количество байт, сообщаем об ошибке.
        cout << "Can't write all string: " << str << endl;
        exit(-1);
    }
}

string readFromPipe(const Pipe& pipe)
{
    // Читаем из pipe.
    auto pipeIndex = size_t(PipeStream::readStream);

    // Длина строки (которая например не больше чем 65к символов).
    uint16_t strLen = 0;

    // Сначала прочтём размер строки (2 байта).
    auto readedSize = read(pipe[pipeIndex], &strLen, sizeof(strLen));

    if (readedSize != sizeof(strLen))
    {
        // Если прочиталось меньшее количество байт, сообщаем об ошибке.
        cout << "Can't read length of string!" << endl;
        exit(-1);
    }

    string resStr(strLen, ' ');

    // Теперь запишем саму строку.
    readedSize = read(pipe[pipeIndex], resStr.data(), strLen);

    if (readedSize != strLen)
    {
        // Если прочиталось меньшее количество байт, сообщаем об ошибке.
        cout << "Can't read all string!" << endl;
        exit(-1);
    }

    return resStr;
}

void closePipe(const Pipe& pipe, const PipeStream &stream)
{
    // Закрываем stream.
    if (close(pipe[size_t(stream)]) == -1)
    {
        cout << "Can't close stream!" << endl;
        exit(-1);
    }
}

int main()
{
    Pipe dataPipe;

    // Попытаемся создать pipe.
    if(pipe(dataPipe.data()) == -1)
    {
        // Если создать pipe не удалось, печатаем об этом сообщение и прекращаем работу.
        cout << "Can't create pipe!" << endl;
        return -1;
    }

    // Семафоры.
    named_semaphore parent_sem (open_or_create,
                                "lab10_3_p_sem", 0
                                );

    named_semaphore child_sem (open_or_create,
                               "lab10_3_c_sem", 0
                               );


    auto result = fork();
    if (result == -1)
    {
        // Если создать процесс не удалось.
        cout << "Can't fork child process!" << endl;
        return -1;
    }
    else if (result > 0)
    {
        // Мы находимся в родительском процессе.

        // Запишем свою строку в pipe.
        string parentStr = "1. Hello, world from Parent!";

        writeToPipe(dataPipe, parentStr);
        parent_sem.post();

        // Прочитаем строку от дочернего процесса.
        child_sem.wait();
        auto readedStr = readFromPipe(dataPipe);

        cout << "[Parent] " << readedStr << endl;

        closePipe(dataPipe, PipeStream::readStream);
        closePipe(dataPipe, PipeStream::writeStream);
        named_semaphore::remove("lab10_3_p_sem");
        named_semaphore::remove("lab10_3_c_sem");
    }
    else
    {
        // Мы находимся в дочернем процессе.

        // Прочитаем строку от родительского процесса.
        parent_sem.wait();
        auto readedStr = readFromPipe(dataPipe);

        cout << "[Child] " << readedStr << endl;

        // Запишем свою строку.
        string childStr = "2. Hello, world from Child!";

        writeToPipe(dataPipe, childStr);
        child_sem.post();
    }

    return 0;
}
