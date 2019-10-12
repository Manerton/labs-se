#include <iostream>
#include "modulo.h"              // -- рабочий класс --
#include "ModuloTest.h"          // -- тестовый класс --

using namespace std;

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №3. Вариант 5.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    ModuloTest test;               // -- объект тестового класса --
    test.run();                     // -- запуск тестовых методов --
    test.report();                  // -- вывод отчета --
    system("pause");
    return 0;
}
