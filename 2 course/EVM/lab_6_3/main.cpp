#include <iostream>
#include <cassert>
#include <cmath>

using namespace std;

void asm_func(double &m, double &n, double &p, double a)   // -- полное ветвление -- //
{
    asm __volatile__ (
            R"(
            fld %[a]        # загрузили a в стек
            fld %[m]        # загрузили m в стек
            fprem
            fstp st(1)
            fldz            # загрузили 0 в стек
            fcomip st, st(1)# сравниваем 0 и остаток от деления и удаляем 0 из стека
            jne not_m       # если остаток от деления != 0, то это не m
            fstp %[m]       # если a делитель m, то сохраним ноль в m
        not_m:
            fld %[a]        # загрузили a в стек
            fld %[n]        # загрузили n в стек
            fprem
            fstp st(1)
            fldz            # загрузили 0 в стек
            fcomip st, st(1)# сравниваем 0 и остаток от деления и удаляем 0 из стека
            jne not_n       # если остаток от деления != 0, то это не n
            fstp %[n]
        not_n:
            fld %[a]        # загрузили a в стек
            fld %[p]        # загрузили p в стек
            fprem
            fstp st(1)
            fldz            # загрузили 0 в стек
            fcomip st, st(1)# сравниваем 0 и остаток от деления и удаляем 0 из стека
            jne not_p       # если остаток от деления != 0, то это не p
            fstp %[p]
        not_p:
            )"
            : [m] "+m" (m), [n] "+m" (n), [p] "+m" (p)
            : [a] "m" (a)
        );
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №6. Задание 3. Вариант 7.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    double m = 30;
    double n = 25;
    double p = 54;
    asm_func(m, n, p, 5);
    if (m == 0) cout << "a является делителем m" << endl; assert(m == 0);
    if (n == 0) cout << "a является делителем n" << endl; assert(n == 0);
    if (p == 0) cout << "a является делителем p" << endl; assert(p == 54);

    cout << endl;

    m = 24;
    n = 16;
    p = 33;
    asm_func(m, n, p, 3);

    if (m == 0) cout << "a является делителем m" << endl; assert(m == 0);
    if (n == 0) cout << "a является делителем n" << endl; assert(n == 16);
    if (p == 0) cout << "a является делителем p" << endl; assert(p == 0);

    cout << endl;

    m = 18;
    n = 133;
    p = 254;
    asm_func(m, n, p, 2);

    if (m == 0) cout << "a является делителем m" << endl; assert(m == 0);
    if (n == 0) cout << "a является делителем n" << endl; assert(n == 133);
    if (p == 0) cout << "a является делителем p" << endl; assert(p == 0);
    return 0;
}
