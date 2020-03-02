#include <iostream>
#include <cassert>
#include <cmath>

using namespace std;

double asm_func(double x)   // -- полное ветвление -- //
{
    double y = 0;    // результат всего выражения
    // -- константы для условий -- //
    const double c2 = 2.0;
    const double c10 = 10.0;
    const double c30 = 30.0;
    asm __volatile__ (
            R"(
            fld %[x]         # загрузили x в стек
            fld %[c2]        # загрузили значение 2 в стек
            fcomip st, st(1) # сравниваем 2 и x (2-x) и убираем значение 2 из стека
            jae first        # если 2 >= x (x <= 2), значит переход к first

            fld %[c10]       # загрузили значение 10 в стек
            fcomip st, st(1) # теперь сравниваем 10 и x (10-x) и убираем 10 из стека
            jae second       # если 10 >= x (x <= 10), то переход к second

            fld %[c30]
            fcomip st, st(1) # теперь сравниваем 30 и x (30-x) и убираем 30 из стека
            jae third        # если 30 >= x (x <= 30), то переход к third

            # обрабатываем случай, когда x > 30
            fsqrt
            fsin
            jmp end
        first:
            fcos    # cos(x)
            fabs    # |cos(x)|
            jmp end
        second:
            fld1
            faddp st(1), st     # x + 1, с удалением 1 из стека
            fld %[x]
            fcos                # cos(x)
            fmul st, st         # cos(x) * cos(x)
            fld1                # загружаем 1
            faddp st(1), st     # 1 + cos^2(x), с удалением 1 из стека
            fdivp st(1), st     # (x+1) / (1 + cos^2(x)), с удалением знаменателя из стека
            jmp end
        third:
            fld1                # загружаем 1
            fld %[c2]           # загружаем 2
            fdivp st(1), st     # 1/2 = 0.5, с удалением 2 из стека
            fmulp st(1), st     # 0.5 * x
            fld1
            fxch st(1)          # меняем 1 и 0.5x местами для fyl2x (теперь 0.5x находится в st(0))
            fyl2x               # log2( 0.5 * x ) * 1
            fldl2t              # загружаем log2( 10 )
            fdivp st(1), st     # log2(0.5*x) / log2(10) -> log10(0.5x)
            jmp end
        end:
            fstp %[y]       # сохраняем результат
            )"
            : [y] "+m" (y)
            : [x] "m" (x), [c2] "m" (c2), [c10] "m" (c10), [c30] "m" (c30)
        );
    return y;
}

double asm_func2(double x) // -- краткая форма ветвления с использованием составных логических выражений -- //
{
    double y = 0;    // результат всего выражения
    // -- константы для условий -- //
    const double c2 = 2.0;
    const double c10 = 10.0;
    const double c30 = 30.0;
    asm __volatile__ (
            R"(
            # первое условие (x <= 2)
            # проверка
            fld %[x]         # загрузили x в стек
            fld %[c2]        # загрузили значение 2 в стек
            fcomip st, st(1)  # сравниваем 2 и x (2-x) и убираем оба значения из стека
            fstp st(0)
            jb secondif      # если 2 < x (x > 2), (т.е противный случай), значит переход к secondif

            # если условию удовлетворяет, то считаем
            fld %[x]         # загрузили x в стек
            fcos             # cos(x)
            fabs             # |cos(x)|
            fstp %[y]        # сохраняем результат
        secondif:
            # второе условие (2 < x <= 10)
            # первая проверка
            fld %[x]            # загрузили x в стек
            fld %[c2]           # загрузили значение 2 в стек
            fcomip st, st(1)    # сравниваем 2 и x (2-x) и убираем оба значения из стека
            fstp st(0)
            jae thirdif         # если 2 >= x (x <= 2), значит переход к thirdif

            # вторая проверка
            fld %[x]            # загрузили x в стек
            fld %[c10]          # загрузили значение 10 в стек
            fcomip st, st(1)              # теперь сравниваем 10 и x (10-x) и убираем оба значения из стека
            fstp st(0)
            jb thirdif          # если 10 < x (x > 10), то переход к thirdif

            # если условию удовлетворяет, то считаем
            fld %[x]            # загрузили x в стек
            fld1
            faddp st(1), st     # x + 1, с удалением 1 из стека
            fld %[x]
            fcos                # cos(x)
            fmul st, st         # cos(x) * cos(x)
            fld1                # загружаем 1
            faddp st(1), st     # 1 + cos^2(x), с удалением 1 из стека
            fdivp st(1), st     # (x+1) / (1 + cos^2(x)), с удалением знаменателя из стека
            fstp %[y]           # сохраняем результат
        thirdif:
            # третье условие (10 < x <= 30)
            # первая проверка
            fld %[x]            # загрузили x в стек
            fld %[c10]          # загрузили значение 10 в стек
            fcomip st, st(1)     # сравниваем 10 и x (10-x) и убираем оба значения из стека
            fstp st(0)
            jae fourif           # если 10 >= x (x <= 10), значит переход к fourif

            # вторая проверка
            fld %[x]            # загрузили x в стек
            fld %[c30]
            fcomip st, st(1)     # теперь сравниваем 30 и x (30-x) и убираем оба значения из стека
            fstp st(0)
            jb fourif           # если 30 < x (x > 30), то переход к fourif

            # если условию удовлетворяет, то считаем
            fld %[x]            # загрузили x в стек
            fld1                # загружаем 1
            fld %[c2]           # загружаем 2
            fdivp st(1), st     # 1/2 = 0.5, с удалением 2 из стека
            fmulp st(1), st     # 0.5 * x
            fld1
            fxch st(1)          # меняем 1 и 0.5x местами для fyl2x (теперь 0.5x находится в st(0))
            fyl2x               # log2( 0.5 * x ) * 1
            fldl2t              # загружаем log2( 10 )
            fdivp st(1), st     # log2(0.5*x) / log2(10) -> log10(0.5x)
            fstp %[y]           # сохраняем результат
        fourif:
            # четвертое условие (x > 30)
            # проверка
            fld %[x]            # загрузили x в стек
            fld %[c30]          # загрузили значение 10 в стек
            fcomip st, st(1)    # сравниваем 30 и x (30-x) и убираем оба значения из стека
            fstp st(0)
            jae endd            # если 30 >= x (x <= 30), то переход к end

            # если условию удовлетворяет, то считаем
            fld %[x]            # загрузили x в стек
            fsqrt
            fsin
            fstp %[y]           # сохраняем результат
        endd:
            )"
            : [y] "+m" (y)
            : [x] "m" (x), [c2] "m" (c2), [c10] "m" (c10), [c30] "m" (c30)
        );
    return y;
}

inline double func(double x)
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

inline double func2(double x)
{
    double y = 0;
    if (x <= 2)
    {
        y = fabs(cos(x));
    }
    if ((x > 2) && (x <= 10))
    {
        y = (x+1) / (1+(cos(x)*cos(x)));
    }
    if ((x > 10) && (x <= 30))
    {
        y = log10(0.5*x);
    }
    if (x > 30)
    {
        y = sin(sqrt(x));
    }
    return y;
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №6. Задание 2. Вариант 7.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    const double eps = 0.00001;
    // -- тест для полного ветвления -- //
    cout << "Первый тест для полного ветвления: " << endl;
    double res = asm_func(1);
    cout << res << endl;
    assert(fabs(res - func(1)) <= eps);

    res = asm_func(2);
    cout << res << endl;
    assert(fabs(res - func(2)) <= eps);

    res = asm_func(5);
    cout << res << endl;
    assert(fabs(res - func(5)) <= eps);

    res = asm_func(10);
    cout << res << endl;
    assert(fabs(res - func(10)) <= eps);

    res = asm_func(20);
    cout << res << endl;
    assert(fabs(res - func(20)) <= eps);

    res = asm_func(30);
    cout << res << endl;
    assert(fabs(res - func(30)) <= eps);

    res = asm_func(35);
    cout << res << endl;
    assert(fabs(res - func(35)) <= eps);

    // -- тест для краткой формы ветвления с использованием составных логических выражений -- //
    cout << "Второй тест для краткой формы ветвления: " << endl;
    res = asm_func2(1);
    cout << res << endl;
    assert(fabs(res - func2(1)) <= eps);

    res = asm_func2(2);
    cout << res << endl;
    assert(fabs(res - func2(2)) <= eps);

    res = asm_func2(5);
    cout << res << endl;
    assert(fabs(res - func2(5)) <= eps);

    res = asm_func2(10);
    cout << res << endl;
    assert(fabs(res - func2(10)) <= eps);

    res = asm_func2(20);
    cout << res << endl;
    assert(fabs(res - func2(20)) <= eps);

    res = asm_func2(30);
    cout << res << endl;
    assert(fabs(res - func2(30)) <= eps);

    res = asm_func2(35);
    cout << res << endl;
    assert(fabs(res - func2(35)) <= eps);
    return 0;
}
