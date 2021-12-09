#include <iostream>
#include "WinSniffer.h"

using namespace std;

int main()
{
    try
    {
        WinSniffer sniffer;
    }
    catch(std::exception& e)
    {
        cout << "Error: " << e.what() << endl;
    }

    system("pause");
    return 0;
}
