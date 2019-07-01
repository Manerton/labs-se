#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <windows.h>


using namespace std;

vector<int> filling_vector_random (int N) // от -50 до 50
{
    vector<int> A(N);
    int chislo;
    for (int i = 0; i < N; i++)
    {
        chislo = -50 + rand() % ( 101 );
        A[i] = chislo;
    }
    return A;
}

void display_vector (vector<int> &v)
{
    for (int &c : v) cout << c << " ";
}

double func_rec(double x, int N) // рекурсивная функция, от N спускается к 0, где x - аргумент функции (x-1)(x+1)
{
    if (N == 0) return x; // если дошел до 0, то возвращаю x
    else
    {
        int step = 2*N+1; // считаю коэф
        return (func_rec(x, N-1) + (pow(x,step) / step)); // считаю сразу сумму как текущий элемент + предыдущий (N-1)
    }
}

double func_rec (double x, double e, double S, double Sn, int n)
// x - аргумент функции (x-1)/(x+1), e - точность, S - сумма, Sn - член ряда на данном шаге, n - шаг
// S - передается в функцию изначально равный x (т.е нулевой член ряда), Sn - так же равен x, n = 1
{
    int step = 2*n+1; // считаю коэф
    Sn = Sn * x * x; // получаю след член ряда без коэф
    if (fabs(Sn/step) > e) // проверяю, чтобы член ряда (уже с коэф) был больше заданной точности
    {
        S += (Sn / step); // прибавляю член ряда к общей сумме
        return (func_rec(x, e, S, Sn, n+1)); // увеличиваю шаг на 1 и вызываю функцию еще раз
    } else return S; // если условие нарушено, то возвращаю сумму
}

double func(double x)
{
    return 0.5 * log(x); // функция для проверки
}

bool cifra_in_chislo (int chislo, int cifra) // рекурсивно проверяю, принадлежит ли заданная цифра числу
{
    if (chislo > 0 && ((chislo % 10) == cifra)) return true;
    else if (chislo == 0) return false;
    else return cifra_in_chislo(chislo / 10, cifra);
}

vector<int> zadanie2 (vector<int> &v, int N, int cifra) // рекурсивно редактирую вектор
{
    if (N == 0) return v;
    else {
        if (cifra_in_chislo(abs(v[N-1]), cifra)) v[N-1] = 0;
        return zadanie2(v, N-1, cifra);
    }
}

bool zadanie3 (int chislo) // рекурсивно определяю, является ли число точной степенью двойки
{
    if (chislo == 1) return true;
    if (chislo % 2 == 0 && chislo != 0) return zadanie3(chislo/2);
    else return false;
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №14. Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
    int menu = -1;
    while (menu != 0)
    {
        do {
            cout << "Меню:" << endl;
            cout << "1. Вычислить рекурсивно Sn, Se, а S обычным способом для проверки." << endl;
            cout << "2. В заданном массиве заменить все числа, содержащие заданную цифру, нулями." << endl;
            cout << "3. Дано натуральное число N. Выведите слово YES, если число N является точной степенью двойки, или слово NO в противном случае." << endl;
            cout << "Введите номер задания для выполнения: ";
            cin >> menu;
        } while (menu < 0 || menu > 3);
        switch (menu)
        {
            case 1: // первое задание
            {
                double a, b, e, x;
                int N;

                cout << "Ввод интервала [a;b] (интервал лежит целиком в интервале [0,2; 1])" << endl;

                cout << "Введите a: ";
                cin >> a;
                while (a < 0.2 || a > 1)
                {
                    cout << "Некорректный ввод." << endl;
                    cout << "Введите a: ";
                    cin >> a;
                }

                cout << "Введите b: ";
                cin >> b;

                while (b <= a || b < 0.2 || b > 1)
                {
                    cout << "Некорректный ввод." << endl;
                    cout << "Введите b: ";
                    cin >> b;
                }

                cout << "Введите N: ";
                cin >> N;

                while (N <= 0 || N > 10)
                {
                    cout << "Некорректный ввод." << endl;
                    cout << "Введите N: ";
                    cin >> N;
                }

                cout << "Введите e: ";
                cin >> e;

                while (e == 0 || e > 0.1 || e < 0)
                {
                    cout << "Некорректный ввод." << endl;
                    cout << "Введите e: ";
                    cin >> e;
                }

                double h = (b-a) / 10;

                int i=1;
                cout << "Задание 1 (вычислить рекурсивно Sn, Se, а S обычным способом для проверки)." << endl;
                cout << "N n/n" << "\t" << "x" << "\t" << "Sn" << "\t\t" << "Se" << "\t\t" << "S" << endl; // заголовок таблицы
                for (x = a; x < b || fabs(x-b) < 0.0001; x += h)
                {
                    double arg = ((x-1) / (x+1)); // высчитываю то, что будет часто повторяться для оптимизации
                    cout << defaultfloat << i << "\t" << x << "\t" << fixed << func_rec(arg, N) << "\t" << func_rec(arg, e, arg ,arg, 1) << "\t" << func(x) << endl;
                    i++;
                }
                system("pause");
                system("cls");
                break;
            }
            case 2: // задание 2.1
            {
                int N;
                cout << "Введите количество элементов массива: ";
                cin >> N;
                vector<int> v = filling_vector_random(N);
                display_vector(v);
                int cifra;
                cout << "\nВведите заданную цифру: ";
                cin >> cifra;
                while (cifra < 0 || cifra > 9)
                {
                    cout << "Вы ввели не цифру!" << endl;
                    cout << "Введите заданную цифру: ";
                    cin >> cifra;
                }
                zadanie2(v, N, cifra);
                display_vector(v);
                cout << endl;
                system("pause");
                system("cls");
                break;

            }
            case 3:
            {
                int N;
                cout << "Введите натуральное число N: ";
                cin >> N;
                while (N < 0)
                {
                    cout << "Вы ввели ненатуральное число" << endl;
                    cout << "Введите натуральное число N: ";
                    cin >> N;
                }
                if (zadanie3(N)) cout << "YES" << endl;
                else cout << "NO" << endl;
                system("pause");
                system("cls");
                break;
            }

        }
    }
}

