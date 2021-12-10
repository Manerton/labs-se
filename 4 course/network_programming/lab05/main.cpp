#include <iostream>
#include "Sniffer.h"

using namespace std;

int main()
{
    try
    {
        Sniffer sniffer;
        sniffer.start();
    }
    catch(std::exception& e)
    {
        cout << "Error: " << e.what() << endl;
    }

    system("pause");
    return 0;
}
