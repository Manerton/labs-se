#define DOCTEST_CONFIG_IMPLEMENT
#include <iostream>
#include "doctest.h"
#include "queue.h"
using namespace std;

int main(int argc, char* argv[] )
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №4. Проект 1. Вариант 8.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;

    doctest::Context context;
    context.applyCommandLine(argc, argv);
    int result = context.run();	// -- пуск системы doctest --
    system("pause");			// -- для просмотра результатов --
    return result;
}
