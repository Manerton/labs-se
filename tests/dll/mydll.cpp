#include "mydll.h"
#include <iostream>

void myDLL::hello()
{
    std::cout << "Hello from DLL!" << std::endl;
}
