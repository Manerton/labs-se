#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h" // DOCTEST

using namespace std;
int main(int argc, char* argv[] )
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №1. Вариант 7.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    int result = context.run();	// -- пуск системы doctest --
    system("pause");			// -- для просмотра результатов --
    return result;
}

