#include <fstream>
#include <iostream>
#include <cmath>
#include <windows.h>

using namespace std;

int main()
{
    system("chcp 1251 > nul");
    int formula;
    double x, y;
    cout << "Лабораторная работа №1. Вариант 13.\n\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
    cout << "Третий способ." << endl;
    cout << "Введите значение x: " << endl;
    cin >> x;
    cout << "Выберите формулу (от 1 до 4): " << endl;
    cin >> formula;

  // Проверка на ввод некорректных данных

    while (formula < 1 || formula > 4)
    {
        cout << "Некорректный ввод\n";
        cout << "Выберите формулу (от 1 до 4): " << endl;
        cin >> formula;
    }
    switch (formula) {
    case 1:
        y = sqrt(abs(x)) + 0.5*sin(x);
        break;
    case 2:
        y = sin(x-5);
        break;
    case 3:
        if (fabs(x+3)< 0.0001 || x+6 < 0)
        {
            cout << "Error: " << "x: " << x << " не входит в область определения функции №" << formula << endl;
            y = 0;
        } else {
            y = sqrt(x+6) / (x+3);
        }
        break;
    case 4:
        if (fabs(x+3)< 0.0001)
        {
             cout << "Error: " << "x: " << x << " не входит в область определения функции №" << formula << endl;
            y = 0;
        } else {
            y = 4.0 / (3+x);
        }
        break;
    }
    cout << "Значение y: " << y << endl;

    ofstream output("Out.txt");
    output << y << endl;
    output.close();
    return 0;
}

