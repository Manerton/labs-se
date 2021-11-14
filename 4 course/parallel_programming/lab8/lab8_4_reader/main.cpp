#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include <string>
#include <iostream>

using namespace std;

int main()
{
    const string filename = "aaa.fifo";

    // Обнуляем маску создания файлов текущего процесса для того, чтобы
    // права доступа у создаваемого FIFO точно соотвествовали параметру вызова mknod().
    umask(0);

    const auto fd = open(filename.c_str(), O_RDONLY);

    if(fd == -1)
    {
        // Если открыть FIFO не удалось, печатаем об этом сообщение и прекращаем работу.

        cout << "Can't open FIFO for reading" << endl;
        return -1;
    }

    // Мы заранее знаем, сколько другая программа хочет передать нам данных.
    const size_t expectedSize = 13;

    // Пробуем прочитать из FIFO expectedSize байт в массив, т.е. всю записанную строку.
    string resString(" ", expectedSize);
    auto actualSize = read(fd, resString.data(), expectedSize);

    if (actualSize != ssize_t(expectedSize))
    {
        // Если прочитать не смогли, сообщаем об ошибке и завершаем работу.

        cout << "Can't read string" << endl;
        return -1;
    }

    // Печатаем прочитанную строку.
    cout << resString << endl;

    // Закрываем read stream и завершаем работу.
    close(fd);

    return 0;
}
