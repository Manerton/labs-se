#include <iostream>
#include "mydll.h"

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    myDLL::hello();
    system("pause");
    return 0;
}
