#include <fstream>
#include <iostream>
#include <cmath>
#include <windows.h>

using namespace std;

int main()
{
    system("chcp 1251 > nul");
    int x;
    double y;
    cout << "Лабораторная работа №1. Вариант 13.\n\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
    cout << "Краткая форма ветвления с использованием составных логических выражений" << endl;

    ifstream input("In.txt");
    input >> x;
    input.close();

    if (x < -12)
    {
        y = sqrt(abs(x)) + 0.5*sin(x);
    }
    if (x < 8 && x >= -12)
    {
        y = sin(x-5);
    }
    if (x < 10 && x >= 8)
    {
        y = sqrt(x+6) / (x+3);
    }
    if (x >= 10)
    {
        y = 4.0 / (3+x);
    }
    cout << "Значение y: " << y << endl;

    ofstream output("Out.txt");
    output << y << endl;
    output.close();
    return 0;
}
