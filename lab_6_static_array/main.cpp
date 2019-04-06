#include <iostream>

using namespace std;

void filling_array_from_keyboard (int A[], const int N)
{
    cout << "Задайте значения элементов массива: " << endl;
    int chislo;
    for (int i = 0; i < N; i++)
    {
        cout << "Для " << i << "-го: ";
        cin >> chislo;
        A[i] = chislo;
    }
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №6 Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
    const int N = 255;
    int A[N];
    return 0;
}
