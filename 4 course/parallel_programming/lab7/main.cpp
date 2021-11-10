#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main(int argc, char *argv[], char *envp[])
{
    // Задание 1 и 2.
    const pid_t chpid = fork();

    if (chpid == -1)
    {
        cout << "Ошибка: не удалось создать дочерний процесс." << endl;
    }
    else if (chpid == 0)
    {
        // Дочерний процесс.
        const pid_t pid = getpid();
        const pid_t ppid = getppid();

        const string chStr = "[Child] ";
        cout << chStr << "PID = " << pid << endl;
        cout << chStr << "Parent PID = " << ppid << endl;

        cout << chStr << "Параметры запуска: " << endl;

        // Задание 3 - параметры запуска.
        for (int i = 0; i < argc; ++i)
        {
            cout << chStr << argv[i] << endl;
        }

        // Задание 3 - параметры окружающей среды.
        cout << chStr << "Параметры окружающей среды (первые 5): " << endl;
        auto it = envp;
        for (int i = 0; i < 5; ++i)
        {
            if (*it == nullptr)
            {
                break;
            }

            cout << chStr << *it++ << endl;
        }

        // Задание 4.
        cout << chStr << "Запускаю программу cat для вывода содержимого файла text.txt" << endl;
        execle("/bin/cat", "/bin/cat", "../lab7/test.txt", 0, envp);

        cout << chStr << "Произошла ошибка при открытии файла" << endl;
        exit(-1);
    }
    else
    {
        // Родительский процесс.
        const pid_t pid = getpid();

        const string pStr = "[Parent] ";
        cout << pStr << "PID = " << pid << endl;
        cout << pStr << "Child PID = " << chpid << endl;

        int a;
        cin >> a;
    }

    return 0;
}
