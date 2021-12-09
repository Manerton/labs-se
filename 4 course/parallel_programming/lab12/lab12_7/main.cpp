#include <cerrno>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <iostream>

using namespace std;

int main()
{
    pid_t pid = -1;

    // В цикле порождаем 5 процессов-детей.
    for (int i = 0; i < 5; ++i)
    {
        if ( (pid = fork()) < 0 )
        {
            cout << "Can't fork child" << i << endl;
            exit(1);
        }
        else if (pid == 0)
        {
            // Child i - завершается с кодом 200 + i.
            exit(200 + i);
        }

        // Продолжение процесса-родителя - уходим на новую итерацию.
    }

    int status = 0;

    // Продолжение процесса-родителя - уходим в цикл и ждем завершения всех потомков.
    while ((pid = wait(&status)) > 0)
    {
        // Анализируем статус завершившегося процесса.
        if ( (status & 0xff) == 0 )
        {
            // Процесс завершился с явным или неявным вызовом функции exit().
            cout << "Process " << pid << " was exited with status " << (status >> 8) << endl;
        }
        else if ((status & 0xff00) == 0)
        {
            // Процесс был завершен с помощью сигнала.
            string str = (status & 0x80) ? "with core file" : "without core file";
            cout << "Process " << pid << " killed by signal " << (status & 0x7f) << str << endl;
        }
    }

    if (pid < 0 && errno != ECHILD)
    {
        // Если возникла ошибка - сообщаем о ней.
        cout << "Some error on wait errno = " << errno;
    }

    return 0;
}
