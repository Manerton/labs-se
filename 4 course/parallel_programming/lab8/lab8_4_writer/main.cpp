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
    string filename = "aaa.fifo";

    // Обнуляем маску создания файлов текущего процесса для того, чтобы
    // права доступа у создаваемого FIFO точно соответствовали параметру вызова mknod().
    umask(0);

    // Попытаемся создать FIFO с именем aaa.fifo в текущей директории.
    if (mknod(filename.c_str(), S_IFIFO | 0666, 0) < 0)
    {
        // Если создать FIFO не удалось, печатаем об этом сообщение и прекращаем работу.

        cout << "Can't create FIFO" << endl;
        return -1;
    }

    auto fd = open(filename.c_str(), O_WRONLY);

    if (fd == -1)
    {
        // Если открыть FIFO не удалось, печатаем об этом сообщение и прекращаем работу.

        cout << "Can't open FIFO for writting" << endl;
        return -1;
    }

    // Пробуем записать в FIFO всю строку "Hello, world!".
    string str = "Hello, world!";
    auto expectedSize = str.size();
    auto actualSize = write(fd, str.c_str(), expectedSize);

    if(actualSize != ssize_t(expectedSize))
    {
        // Если записалось меньшее количество байт, сообщаем об ошибке и завершаем работу.
        cout << "Can't write all string to FIFO" << endl;
        return -1;
    }

    close(fd);

    // Удалим FIFO файл
    remove(filename.c_str());

    return 0;
}
