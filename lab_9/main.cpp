#include <iostream>
#include <fstream>
#include <random>
#include <ctime>

using namespace std;

void ex1 (int N, double min, double max)
{
    mt19937 gen; // создаю генератор сч
    gen.seed(time(0));
    uniform_real_distribution<> urd(min,max); // указываю диапазон генерирования

    ofstream toBin("input.bin", ios::binary);
    for (int i = 0; i < N; i++)
    {
        double chislo = urd(gen);
        toBin.write((char*)&chislo, sizeof(double));
    }
    toBin.close();
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №9 Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;

    int N;
    cout << "Введите количество чисел N: ";
    cin >> N;
    while (N < 1 || N > 1000000)
    {
        cout << "error!" << endl;
        cout << "Введите количество чисел N: ";
        cin >> N;
    }

    double min, max;
    cout << "Укажите диапазон случайных вещественных чисел.";
    cout << "Нижняя граница: ";
    cin >> min;
    cout << "Верхняя граница: ";
    cin >> max;
    while (max < min)
    {
        cout << "error!" << endl;
        cout << "Верхняя граница: ";
        cin >> max;
    }
    ex1(N,min,max);
    return 0;
}
