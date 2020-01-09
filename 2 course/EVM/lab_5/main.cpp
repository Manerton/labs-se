#include <iostream>
#include <cassert>
#include <cmath>

using namespace std;

double asm_func(double a, double b)
{
    double res = 0;    // результат всего выражения

    asm __volatile__ (
        R"(
        fld1          # загружаем 1.0 - нужна для fyl2x
        fld %[a]      # скопировали a в st(0) (загрузить a в st(0))
        fld %[b]      # скопировали b в st(0) (загрузить b в st(0))
        faddp         # st(1) = st(0) + st(1): (a+b), а после st(1) -> st(0) (бывший st(0) выталкивается)
        fyl2x         # 1.0*log2(a+b)
        fldln2        # загружаем константу ln(2)
        fmulp         # получаем по формуле ln(a+b) = ln(2) * log2(a+b)
                      # st(1) = st(0) * st(1), а после st(1) -> st(0) (бывший st(0) выталкивается)
        #----------------------------------
        # ищем e^a
        # будем искать по формуле e^a=2^(a*log2(e))
        fld %[a]      # скопировали a в st(0) (загрузить a в st(0))
        fldl2e        # загружаем константу log2(e)
        fmulp         # st(1) = log2(e) * a, а после st(1) -> st(0) (бывший st(0) выталкивается)
        fld ST        # загружаем копию из ST(0)
        frndint       # округляем копию до целого
        fsub st(1), st # вычитаем из дробного числа целое и получаем дробную часть в st(1)
        fxch          # меняем местами st(0) и st(1), таким образом в st(0) - дробная часть
        f2xm1         # возводим 2 в степень st(0) (где дробная часть) и отнимаем от результата 1
        fld1          # загружаем 1.0
        faddp          # прибавляем 1, т.е возвращаем единичку обратно
        fscale        # теперь число в st(0) умножаем на 2^st(1) (Масштабирование на степень числа 2)
        # таким образом мы возвели 2 в степень равной дробной части
        # и умножили это на число = 2 в степени целой части
        # что эквивалентно 2 ^ все дробное число = a*log2(e)
        # функция f2xm1 не дает возвести сразу, так как диапазон ее аргумента от 0 до 1
        fstp st(1)    # функция fscale не выталкивает из стека значение порядка, делаем это вручную
        #----------------------------------
        fld %[b]      # скопировали b в st(0), в st(1) лежит e^a, а в st(2): ln(a+b)
        fsin          # синус от b
        fmulp         # st(1) = sin(b) * e^a, а после st(1) -> st(0) (бывший st(0) выталкивается)
        fsubp         # st(1) = ln(a+b) - e^a*sin(b), т.е получаем числитель в st(0)
        #----------------------------------
        fld %[a]      # скопировали a в st(0) (загрузить a в st(0))
        fld %[b]      # скопировали b в st(0) (загрузить b в st(0))
        fsubp         # st(1) = st(1) - st(0): (a-b), а после st(1) -> st(0) (бывший st(0) выталкивается)
        fcos          # считаем cos от (a-b)
        fld %[a]      # скопировали a в st(0) (загрузить a в st(0))
        fsin          # получили sin (a)
        fld %[b]      # скопировали a в st(0) (загрузить a в st(0))
        fsin          # получили sin (b)
        fmulp         # st(1) = sin(a) * sin(b), а после st(1) -> st(0) (бывший st(0) выталкивается)
        fsubp         # st(1) = cos(a-b) - sin(a)*sin(b), а после st(1) -> st(0) (бывший st(0) выталкивается)
        # получили знаменатель
        #----------------------------------
        fdivp        # st(1) = st(1) / st(0): (chis1) / (chis2), а после st(1) -> st(0)
        fstp %[res]  # выгружаем результат в переменную res
        )"
        : [res] "+m" (res)
        : [a] "m" (a), [b] "m" (b)
    );
    return res;
}

inline double func(double a, double b)
{
    double chis = log(a+b) - exp(a) * sin(b);
    double znam = cos(a-b) - sin(a)* sin(b);
    return chis/znam;
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №4. Проект 2. Вариант 2.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;

    double res = asm_func(5,1);
    cout << res << endl;
    assert(fabs(res - func(5,1)) <= 0.00001);

    res = asm_func(3.2,1.7);
    cout << res << endl;
    assert(fabs(res - func(3.2,1.7)) <= 0.00001);

    res = asm_func(2,7.8);
    cout << res << endl;
    assert(fabs(res - func(2,7.8)) <= 0.00001);

    res = asm_func(10,5);
    cout << res << endl;
    assert(fabs(res - func(10,5)) <= 0.00001);

    res = asm_func(4.1,3.14);
    cout << res << endl;
    assert(fabs(res - func(4.1,3.14)) <= 0.00001);
    return 0;
}
