#include <iostream>
#include <cassert>
#include <cmath>

using namespace std;

float asm_func(float x, float y)
{
    float res = 0;    // результат всего выражения
    const float c10 = 10;  // -- константа-коэффициент -- //
    const float c8 = 8;  // -- константа-коэффициент -- //
    const float c5 = 5;  // -- константа-коэффициент -- //
    const float c3 = 3;  // -- константа-коэффициент -- //
    asm __volatile__ (
        R"(
        fld %[x]              # загрузим x в st(0)
        fmul %[c5]            # умножим st(0) на 5 и получим 5x
        fld %[y]              # загрузим y в st(0), x перешел в st(1)
        fmul %[c10]           # умножим st(0) на 10 и получим 10y
        fsubp                 # st(1) = st(1) - st(0): (5x - 10y), а после st(1) -> st(0) (бывший st(0) выталкивается)

        fld %[y]              # загрузим y в st(0)
        fadd ST, ST(0)        # сложим y+y = 2y
        fdivp                 # st(1) = st(1) / st(0): (5x-10y) / 2y, а после st(1) -> st(0)

        fld %[y]              # загрузим y в st(0), результат деления первой дроби ушел в st(1)
        fmul %[c3]            # умножим st(0) на 3 и получим 3y
        fadd ST, ST(0)        # сложим 3y+3y = 6y
        fld %[x]              # помещаем x в st(0), 6y уходит в st(1), результат деления первой дроби ушел в st(2)
        fmul %[c3]            # умножаем на 3 и получаем 3x
        fsubp                 # st(1) = st(1) - st(0): (6y - 3x), а после st(1) -> st(0), st(2) -> st(1)

        fld %[y]              # загрузим y в st(0), (6y-3x) уходит в st(1), рез. деления 1 дроби ушел в st(2)
        fmul %[c8]            # умножим st(0) на 8 и получим 8y
        fmul %[y]             # умножим st(0) на y и получим 8(y^2)
        fdivp                 # st(1) = st(1) / st(0): (6y-3x) / (8y^2), а после st(1) -> st(0), st(2) -> st(1)
                              # в данный момент st(1) лежит результат деления первой дроби, а в st(0) - второй
        fdivp                 # st(1) = st(1) / st(0): (drob1) / (drob2), а после st(1) -> st(0)
        fstp %[res]           # сохраняем результат всей функции, выталкивая st(0), таким образом стек чист
        )"
        : [res] "+m" (res)
        : [x] "m" (x), [y] "m" (y), [c10] "m" (c10), [c8] "m" (c8), [c5] "m" (c5), [c3] "m" (c3)
    );
    return res;
}

inline float func(float x, float y)
{
    return ((5*x - 10*y)/(y+y))/((6*y-3*x)/(8*y*y));
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №4. Проект 2. Вариант 2.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;

    float res = asm_func(5,1);
    assert(fabs(res - func(5,1)) <= 0.00001f);
    cout << res << endl;

    res = asm_func(2,-1);
    assert(fabs(res - func(2,-1)) <= 0.00001f);
    cout << res << endl;

    res = asm_func(-1,1);
    assert(fabs(res - func(-1,1)) <= 0.00001f);
    cout << res << endl;

    res = asm_func(33,2);
    assert(fabs(res - func(33,2)) <= 0.00001f);
    cout << res << endl;

    res = asm_func(3,-5);
    assert(fabs(res - func(3,-5)) <= 0.00001f);
    cout << res << endl;

    res = asm_func(-3,5);
    assert(fabs(res - func(-3,5)) <= 0.00001f);
    cout << res << endl;

    return 0;
}
