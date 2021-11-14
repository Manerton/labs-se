#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include <string>
#include <iostream>

using namespace std;

int writeToFile(const string& filename, const string& str)
{
    // Обнуляем маску создания файлов текущего процесса для того, чтобы
    // права доступа у создаваемого файла точно соотвествовали параметру вызова open().
    umask(0);

    // Попытаемся открыть файл с именем myfile в текущей директории только для записи.
    // Если файл не существует, попробуем его создать с правами
    // доступа 0666, т. е. read-write для всех категорий пользователей.

    /// Файловый дескриптор.
    auto fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);

    // Если файл открыть не удалось, печатаем об этом сообщение и прекращаем работу.
    if (fd == -1)
    {
        cout << "Can't open file" << endl;
        return -1;
    }

    // Пробуем записать в файл всю строку "Hello, world!".
    auto expectedSize = str.size();
    auto actualSize = write(fd, str.c_str(), expectedSize);

    // Если записалось меньше, чем ожидалось, то сообщаем об ошибке.
    if (actualSize != ssize_t(expectedSize))
    {
        cout << "Can't write all string" << endl;
        return -1;
    }

    // Закрываем файл.
    if (close(fd) == -1)
    {
        cout << "Can't close file" << endl;
        return -1;
    }

    return 0;
}

int readFromFile(const string& filename, string& readedStr)
{
    // Теперь прочтём из файла.
    auto fd = open(filename.c_str(), O_RDONLY);

    // Если файл открыть не удалось, печатаем об этом сообщение и прекращаем работу.
    if (fd == -1)
    {
        cout << "Can't open file" << endl;
        return -1;
    }

    string buf = " ";

    ssize_t readedSize = read(fd, buf.data(), 1);

    while (readedSize > 0)
    {
        readedStr.append(buf);
        readedSize = read(fd, buf.data(), 1);
    }

    if (readedSize == -1)
    {
        cout << "Can't read all string" << endl;
        return -1;
    }

    // Закрываем файл.
    if (close(fd) == -1)
    {
        cout << "Can't close file" << endl;
        return -1;
    }

    return 0;
}

int main()
{
    auto res = writeToFile("myfile", "Hello, world!\n");

    if (res == -1)
    {
        return -1;
    }

    string readedStr = "";
    auto res2 = readFromFile("myfile", readedStr);

    if (res2 == -1)
    {
        return -1;
    }

    cout << readedStr << endl;

    return 0;
}
