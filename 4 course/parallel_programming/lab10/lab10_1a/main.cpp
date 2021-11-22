// Эта программа получает доступ к одному системному семафору,
// ждет пока его значение не станет больше 0
// после запуска программы 2, а затем уменьшает его на 1.

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


        cout << "Semaphore waits." << endl;

        for (int i = 0; i < 5; ++i)
        {
            sem.wait();
        }

        cout << "Semaphore unlocked!" << endl;

        named_semaphore::remove("lab10_sem");
    }
    catch (interprocess_exception &ex)
    {
        cout << "Error: " << ex.what() << endl;
        return 1;
    }

    return 0;

}
