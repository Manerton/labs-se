#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "cpu.h"

using namespace std;

bool Uploader(CPU &cpu, const string &filename)
{
    ifstream file(filename);

    if (!file.is_open()) return false;

    uint16_t startAddress = 0;  
    char symbol = ' ';
    while (symbol != 'e')
    {
        string s;
        getline(file, s);
        if (!s.empty())
        {

        }
    }
}

int main (int argc, char* argv[])
{
    //CPU cpu;
    if (argc > 1)
    {
        cout << "File " << argv[1] << " is ";

    } else cout << "Programm has been started without arguments." << endl;
    system("pause");


    return 0;
}

