#include <iostream>
#include <money.h>
using namespace std;

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №4. Вариант 3.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    Money money(128,37);
    cout << (money /= 22.217);
    return 0;
}
