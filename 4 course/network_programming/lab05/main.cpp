#include <iostream>
#include "WinSniffer.h"

using namespace std;

int main()
{
    try
    {
        WinSniffer sniffer;
        sniffer.start();
    }
    catch(std::exception& e)
    {
        cout << "Error: " << e.what() << endl;
    }

    system("pause");
    return 0;
}
