#include <iostream>
#include <cassert>
#include <cmath>

using namespace std;

double asm_func(double x)
{
    double y = 0;    // результат всего выражения
    const double c4 = 4.0; // -- это для противного случая -- //
    const double c3 = 3.0; // -- это для условия -- //
    // -- условие y = x^2, если x >= 0 и x <= 3 -- //
    // -- иначе y = 4 -- //
    asm __volatile__ (
            R"(
            fld %[c3]       # загрузили значение 3 в стек
            fldz            # загрузили значение 0 в стек
            fld %[x]        # загрузили x в стек
            fcomi st, st(1) # сравниваем x и 0 (x-0)
            fstp st(1)      # убираем значение 0 из стека
            jb else         # если x < 0 (нарушено условие), значит переход к else
            fcomi st, st(1) # теперь сравниваем x и 3 (x-3)
            fstp st(1)      # убираем значение 3 из стека
            ja else         # если x > 3 (нарушено условие), то переход к else
            fmul st, st     # получаем x^2
            jmp end
        else:
            fld %[c4]       # загрузили значение 4 в стек
            fstp st(1)      # убираем x из стека
        end:
            fstp %[y]       # сохраняем результат
            )"
            : [y] "+m" (y)
            : [x] "m" (x), [c4] "m" (c4), [c3] "m" (c3)
        );
    return y;
}

inline double func(double x)
{
    if (x >= 0 && x <= 3) return x*x;
    return 4.0;
}

inline double func2(double x)
{
    double y = 0;
    if (x <= 2)
    {
        y = fabs(cos(x));
    }
    else if (x <= 10)
    {
        y = (x+1) / (1+(cos(x)*cos(x)));
    }
    else if (x <= 30)
    {
        y = log10(0.5*x);
    }
    else if (x > 30)
    {
        y = sin(sqrt(x));
    }
    return y;
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №4. Проект 2. Вариант 2.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;

    double res = asm_func(1);
    cout << res << endl;
    assert(fabs(res - func(1)) <= 0.00001);

    res = asm_func(2);
    cout << res << endl;
    assert(fabs(res - func(2)) <= 0.00001);

    res = asm_func(3);
    cout << res << endl;
    assert(fabs(res - func(3)) <= 0.00001);

    res = asm_func(4);
    cout << res << endl;
    assert(fabs(res - func(4)) <= 0.00001);

    res = asm_func(-2);
    cout << res << endl;
    assert(fabs(res - func(-2)) <= 0.00001);
    return 0;
}
