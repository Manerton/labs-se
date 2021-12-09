// Программа с пользовательской обработкой сигнала SIGINT.

#include <signal.h>
#include <iostream>

using namespace std;

// Функция my_handler - пользовательский обработчик сигнала.

void my_handler(int nsig)
{
    if (nsig == SIGINT)
    {
        cout << "Receive signal " << nsig << ", CTRL-C pressed" << endl;
    }
    else if (nsig == SIGQUIT)
    {
        cout << "Receive signal " << nsig << ", CTRL+4 pressed" << endl;
    }
}

int main()
{
    // Выставляем реакцию процесса на сигнал SIGINT.
    signal(SIGINT, my_handler);
    signal(SIGQUIT, my_handler);

    // Начиная с этого места, процесс будет печатать сообщение о возникновении сигнала SIGINT.
    while(1);
    return 0;
}
