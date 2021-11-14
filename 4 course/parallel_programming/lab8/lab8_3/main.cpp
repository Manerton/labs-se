#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include <array>
#include <string>
#include <iostream>

using namespace std;

using Pipe = array<int, 2>;

void writeToPipeAndClose(const Pipe& pipe, const string& str)
{
    // Закроем read stream.
    close(pipe[0]);

    // Пробуем записать в pipe всю строку.
    auto expectedSize = str.size();
    auto actualSize = write(pipe[1], str.c_str(), expectedSize);
    if (actualSize != ssize_t(expectedSize))
    {
        // Если записалось меньшее количество байт, сообщаем об ошибке.
        cout << "Can't write all string: " << str << endl;
        exit(-1);
    }

    // Закрываем write stream.
    if (close(pipe[1]) == -1)
    {
        cout << "Can't close write stream with str: " << str << endl;
        exit(-1);
    }
}

void readFromPipeAndClose(const Pipe& pipe, string& str)
{
    // Закроем write stream.
    close(pipe[1]);

    // Пробуем прочитать из pip'а expectedSize байт в другой массив, т.е. всю записанную строку.
    auto expectedSize = str.size();
    auto actualSize = read(pipe[0], str.data(), expectedSize);

    if (actualSize != ssize_t(expectedSize))
    {
        // Если прочитать не смогли, сообщаем об ошибке.
        cout << "Can't read all string" << endl;
        exit(-1);
    }

    // Закрываем read stream.
    if (close(pipe[0]) == -1)
    {
        cout << "Can't close read stream" << endl;
        exit(-1);
    }
}

int main()
{
    Pipe parentWritePipe;
    string parentStr = "Hello, world from Parent!";

    Pipe childWritePipe;
    string childStr = "Hello, world from Child!";

    // Попытаемся создать первый pipe.
    if(pipe(parentWritePipe.data()) == -1)
    {
        // Если создать pipe не удалось, печатаем об этом сообщение и прекращаем работу.
        cout << "Can't create first pipe" << endl;
        return -1;
    }

    // Попытаемся создать второй pipe.
    if(pipe(childWritePipe.data()) == -1)
    {
        // Если создать pipe не удалось, печатаем об этом сообщение и прекращаем работу.
        cout << "Can't create second pipe" << endl;
        return -1;
    }

    auto result = fork();
    if (result == -1)
    {
        // Если создать процесс не удалось.
        cout << "Can't fork child process" << endl;
        return -1;
    }
    else if (result > 0)
    {
        // Мы находимся в родительском процессе.
        // Запишем свою строку и прочитаем строку от дочернего процесса.

        writeToPipeAndClose(parentWritePipe, parentStr);

        // Допустим заранее договорились с дочерним процессом о размере читаемой строки.
        string readedStr(" ", childStr.size());
        readFromPipeAndClose(childWritePipe, readedStr);
        cout << "[Parent] " << readedStr << endl;
    }
    else
    {
        // Мы находимся в дочернем процессе.
        // Запишем свою строку и прочитаем строку от родительского процесса.

        writeToPipeAndClose(childWritePipe, childStr);

        // Допустим заранее договорились с родительским процессом о размере читаемой строки.
        string readedStr(" ", parentStr.size());
        readFromPipeAndClose(parentWritePipe, readedStr);
        cout << "[Child] " << readedStr << endl;
    }

    return 0;
}
