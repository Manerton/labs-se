// Эта программа получает доступ к системному семафору
// и увеличивает его значение на 1, чтобы разблокировать программу 1

#include <iostream>
#include <string>
#include <algorithm>

#include <boost/interprocess/sync/named_semaphore.hpp>

using namespace std;
using namespace boost::interprocess;

int main()
{
    try
    {
        named_semaphore sem (open_or_create,
                             "lab10_sem", 0
                             );

        cout << "Semaphore increase." << endl;

        sem.post();

        cout << "Semaphore increased!" << endl;
    }
    catch (interprocess_exception &ex)
    {
        cout << "Error: " << ex.what() << endl;
        return 1;
    }

    return 0;

}
