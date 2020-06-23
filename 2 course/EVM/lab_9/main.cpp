#include <iostream>
#include <cmath>
#include <tuple>
#include <cassert>

using namespace std;

double zadanie1_cpp(double a, double x, double y)
{
    double first_slag = (x*x - x*y) / (9*x*x - 18*x*y+9*y*y);
    double second_slag = (a*a*a*a - a) / (3*a*a + 3*a + 3);
    return first_slag + second_slag;
}

// параметры передавать в регистрах сопроцессора
double zadanie1_asm(double a, double x, double y)
{
    double *result = new double(); // здесь будет записан результат
    double first_slag = 0;
    // коэффициенты
    double c9 = 9.0;
    double c18 = -18.0;
    double c3 = 3.0;
    asm __volatile__ (
        R"(
        # загружаем в регистры сопроцессора
        fld %[a]
        fld %[x]
        fld %[y]
        mov rbx, %[result]  # помещаем в rbx адрес, который лежит в указателе

        call Calculate

        jmp finish # завершаем работу

        Calculate: # подпрограмма
            fld st(1)       # загрузили значение x (x y x a) в скобках значения в стеке сопроцессора
            fmul st, st     # умножаем x*x (x*x y x a)
            fld st(2)       # загрузили x (x x*x y x a)
            fmul st, st(2)  # (x*y x*x y x a)
            fsubp st(1), st # ((x*x - x*y) y x a) числитель первой дроби
            fld st(2)       # (x (x*x - x*y) y x a)
            fmul st, st     # (x*x (x*x - x*y) y x a)
            fld %[c9]       # (9 x*x (x*x - x*y) y x a)
            fmulp st(1), st # (9*x*x (x*x - x*y) y x a)
            fld st(2)       # (y 9*x*x (x*x - x*y) y x a)
            fmul st, st     # (y*y 9*x*x (x*x - x*y) y x a)
            fld %[c9]
            fmulp st(1), st # (9*y*y 9*x*x (x*x - x*y) y x a)
            fld st(4)
            fld st(4)
            fmulp st(1), st
            fld %[c18]
            fmulp st(1), st # (-18*x*y 9*y*y 9*x*x (x*x - x*y) y x a)
            faddp st(1), st # ((-18*x*y + 9*y*y) 9*x*x (x*x - x*y) y x a)
            faddp st(1), st # ((9*x*x - 18*x*y+9*y*y) (x*x - x*y) y x a)
            fdivp st(1), st # (((x*x - x*y) / (9*x*x - 18*x*y+9*y*y)) y x a)
            fstp %[first_slag] # (y x a)

            fld st(2)
            fmul st, st(3)
            fmul st, st(3)
            fmul st, st(3) # (a*a*a*a y x a)
            fld st(3)
            fsubp st(1), st # числитель второй дроби
            fld st(3)
            fld %[c3] # (3 a a*a*a*a y x a)
            fmul st(0), st(1) # (3*a a a*a*a*a y x a)
            fmul st(1), st    # (3*a 3*a*a a*a*a*a y x a)
            fld %[c3]
            faddp st(1), st
            faddp st(1), st # ((3 + 3*a + 3*a*a) a*a*a*a y x a)
            fdivp st(1), st # вторая дробь
            fld %[first_slag]   # загружаем первую
            faddp st(1), st
            fstp qword ptr[rbx] # записываем результат по адресу в rbx
            ret # возврат из подпрограммы

         finish:
            # очистим стек сопроцессора
            fstp st
            fstp st
            fstp st
        )"
        : [first_slag] "+m" (first_slag), [result] "+m" (result)
        : [a] "m" (a), [x] "m" (x), [y] "m" (y), [c3] "m" (c3), [c9] "m" (c9), [c18] "m" (c18)
    );
    return *result;
}

float zadanie2_cpp(float x)
{
    float inner = (sin(3*x) + pow(2,x));
    return log(inner);
}

// параметры типа float передавать в стеке
float zadanie2_asm(float x)
{
    float *result = new float(); // здесь будет записан результат
    double c3 = 3.0;
    asm __volatile__ (
        R"(
        # заносим x в стек
        push qword ptr %[x]
        mov rbx, %[result]  # помещаем в rbx адрес, который лежит в указателе

        call Calculate2

        jmp finish2 # завершаем работу

        Calculate2:         # подпрограмма
            pop rax                 # сохраняем адрес возврата в rax
            fld dword ptr[rsp]      # загрузили значение x из стека
            fld st(0)       # загружаем копию x
            frndint         # округляем копию до целого
            fsub st(1), st  # вычитаем из дробного числа целое и получаем дробную часть в st(1)
            fxch            # меняем местами st(0) и st(1), таким образом в st(0) - дробная часть
            f2xm1           # возводим 2 в степень st(0) (где дробная часть) и отнимаем от результата 1
            fld1            # загружаем 1.0
            faddp           # прибавляем 1, т.е возвращаем единичку обратно
            fscale          # теперь число в st(0) умножаем на 2^st(1) (Масштабирование на степень числа 2)
            # таким образом мы возвели 2 в степень равной дробной части
            # и умножили это на число = 2 в степени целой части
            # что эквивалентно 2 ^ все дробное число x
            # функция f2xm1 не дает возвести сразу, так как диапазон ее аргумента от 0 до 1
            fstp st(1)      # функция fscale не выталкивает из стека значение порядка, делаем это вручную

            fld dword ptr[rsp]   # загружаем x из стека
            fld %[c3]
            fmulp st(1), st
            fsin            # получили sin (3x)
            faddp st(1), st # получили sin(3x) + 2^x

            ### ищем теперь ln от выражения выше
            fld1            # загружаем 1.0 - нужна для fyl2x
            fxch st(1)      # поменяем содержимое регистров местами, так как для fyl2x важен порядок
            fyl2x           # 1.0*log2(sin(3x) + 2^x)
            fldln2          # загружаем константу ln(2)
            fmulp st(1),st  # получаем искомое по формуле ln(a) = ln(2) * log2(a)

            fstp dword ptr[rbx] # записываем результат по адресу в rbx
            push rax            # возвращаем адрес возврата в стек
            ret # возврат из подпрограммы

         finish2:
            pop rax
        )"
        : [result] "+m" (result)
        : [x] "m" (x), [c3] "m" (c3)
    );
    return *result;
}

auto zadanie3_cpp(float rad)
{
    double PI = M_PI;
    double float_deg = (180.0 / PI) * rad;
    int deg = trunc(float_deg);
    double float_min = (float_deg-deg) * 60;
    int min = trunc(float_min);
    double sec = (float_min-(min))*60;
    return std::tuple<int,int,float>(deg, min, sec);
}

// в стеке передавать адреса параметров
auto zadanie3_asm(float x)
{
    int *deg = new int();
    int *min = new int();
    float *sec = new float();
    float *x_ptr = &x;
    double c180 = 180.0;
    double c60 = 60.0;
    double PI = M_PI;
    asm __volatile__ (
        R"(
        # заносим x в стек
        push qword ptr %[x]

        call Calculate3

        jmp finish3 # завершаем работу

        Calculate3:         # подпрограмма
            pop rax                 # сохраняем адрес возврата в rax
            fld %[c180];
            fld %[PI];
            fdivp st(1), st         # 180/PI
            # в стеке лежит адрес, поместим содержимое по этому адресу в rcx
            mov rcx, [rsp]
            # загрузим в сопроцессор значение из rcx
            fld dword ptr[rcx]
            fmulp st(1), st         # (180/Pi) * rad
            fld st(0)               # делаем копию

            mov rbx, %[deg]         # помещаем в rbx адрес, который лежит в указателе
            FISTTP dword ptr[rbx]   # отбрасываем дробную часть для градусов

            FILD dword ptr[rbx]     # загружаем целую часть
            fsubp st(1), st(0)      # находим дробную
            fld %[c60]
            fmulp st(1), st
            fld st(0)               # делаем копию

            mov rbx, %[min]         # помещаем в rbx адрес, который лежит в указателе
            FISTTP dword ptr[rbx]   # отбрасываем дробную часть для минут

            FILD dword ptr[rbx]     # загружаем целую часть от минут
            fsubp st(1), st(0)      # находим дробную
            fld %[c60]
            fmulp st(1), st

            mov rbx, %[sec]         # помещаем в rbx адрес, который лежит в указателе
            fstp dword ptr[rbx]     # записываем результат по адресу в rbx

            push rax                # возвращаем адрес возврата в стек
            ret # возврат из подпрограммы

         finish3:
            pop rax
        )"
        : [deg] "+m" (deg), [min] "+m" (min), [sec] "+m" (sec)
        : [x] "m" (x_ptr), [c180] "m" (c180), [PI] "m" (PI), [c60] "m" (c60)
    );
    return std::tuple<int,int,float>(*deg, *min, *sec);
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №9. Вариант 7.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    const double eps = 0.00001; // -- точность сравнения при ассерте -- //
    cout << "Первое задание: " << endl;
    double res1_cpp = zadanie1_cpp(2,3,4);
    double res1_asm = zadanie1_asm(2,3,4);
    assert(fabs(res1_cpp - res1_asm) <= eps);
    cout << res1_cpp << endl;
    cout << res1_asm << endl;

    res1_cpp = zadanie1_cpp(7,4,9);
    res1_asm = zadanie1_asm(7,4,9);
    assert(fabs(res1_cpp - res1_asm) <= eps);
    cout << res1_cpp << endl;
    cout << res1_asm << endl;

    res1_cpp = zadanie1_cpp(5,3,14);
    res1_asm = zadanie1_asm(5,3,14);
    assert(fabs(res1_cpp - res1_asm) <= eps);
    cout << res1_cpp << endl;
    cout << res1_asm << endl;

    cout << "Второе задание: " << endl;
    double res2_cpp = zadanie2_cpp(3.2);
    double res2_asm = zadanie2_asm(3.2);
    assert(fabs(res1_cpp - res1_asm) <= eps);
    cout << res2_cpp << endl;
    cout << res2_asm << endl;

    res2_cpp = zadanie2_cpp(17);
    res2_asm = zadanie2_asm(17);
    assert(fabs(res1_cpp - res1_asm) <= eps);
    cout << res2_cpp << endl;
    cout << res2_asm << endl;

    res2_cpp = zadanie2_cpp(0.04);
    res2_asm = zadanie2_asm(0.04);
    assert(fabs(res1_cpp - res1_asm) <= eps);
    cout << res2_cpp << endl;
    cout << res2_asm << endl;

    cout << "Третье задание: " << endl;
    auto [deg1, min1, sec1] = zadanie3_cpp(3);
    auto [deg2, min2, sec2] = zadanie3_asm(3);
    cout << deg1 << " " << min1 << " " << sec1 << endl;
    cout << deg2 << " " << min2 << " " << sec2 << endl;
    assert(deg1 == deg2);
    assert(min1 == min2);
    assert(fabs(sec1 - sec2) <= eps);

    auto [deg3, min3, sec3] = zadanie3_cpp(7.43);
    auto [deg4, min4, sec4] = zadanie3_asm(7.43);
    cout << deg3 << " " << min3 << " " << sec3 << endl;
    cout << deg4 << " " << min4 << " " << sec4 << endl;
    assert(deg3 == deg4);
    assert(min3 == min4);
    assert(fabs(sec3 - sec4) <= eps);
    return 0;
}
