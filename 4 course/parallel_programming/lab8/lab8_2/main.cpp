#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include <array>
#include <string>
#include <iostream>

using namespace std;

int main()
{
    array<int, 2> fd;
    string str = "Hello, world!";
    string resStr(" ", str.size());

    // Попытаемся создать pipe.
    if(pipe(fd.data()) == -1)
    {
        // Если создать pipe не удалось, печатаем об этом сообщение и прекращаем работу.
        cout << "Can't create pipe" << endl;
        return -1;
    }

    // Пробуем записать в pipe всю строку "Hello, world!".


    auto expectedSize = str.size();
    auto actualSize = write(fd[1], str.c_str(), expectedSize);

    if (actualSize != ssize_t(expectedSize))
    {
        // Если записалось меньшее количество байт, сообщаем об ошибке.

        cout << "Can't write all string" << endl;
        return -1;
    }

    // Пробуем прочитать из pip'а expectedSize байт в другой массив, т.е. всю записанную строку.

    actualSize = read(fd[0], resStr.data(), expectedSize);

    if((actualSize != ssize_t(expectedSize)) || actualSize == -1)
    {
        // Если прочитать не смогли, сообщаем об ошибке.
        cout << "Can't read all string" << endl;
        return -1;
    }

    // Печатаем прочитанную строку.
    cout << resStr << endl;

    // Закрываем входной поток.
    if (close(fd[0]) == -1)
    {
       cout << "Can't close input stream" << endl;
       return -1;
    }

    // Закрываем выходной поток.
    if (close(fd[1]) == -1)
    {
       cout << "Can't close output stream" << endl;
       return -1;
    }

    return 0;
}
