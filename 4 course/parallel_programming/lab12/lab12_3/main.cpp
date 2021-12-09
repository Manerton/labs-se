// Программа, игнорирующая сигнал SIGINT и SIGQUIT.

#include <signal.h>

int main()
{
    // Выставляем реакцию процесса на сигнал SIGINT и SIGQUIT на игнорирование.

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    // Начиная с этого места, процесс будет игнорировать возникновение сигнала SIGINT.

    while(1);
    return 0;
}
