#include <iostream>
#include <cassert>
#include <cmath>
#include <iomanip>

using namespace std;

double asm_func(int n, double x)
{
    const double cneg1 = -1.0; // -- это для коэф -- //
    const double c2 = 2.0;

    double i = 1.0; // -- текущий шаг -- //
    double result = 1;  // -- переменная - сумма ряда при данном x и n; значение 1 здесь при n = 0 -- //
    double slag_koef = 1;  // -- коэф. текущего члена суммы -- //
    double slag_chis = 1;  // -- числитель текущего члена суммы -- //
    double slag_znam = 1;  // -- знаменатель -- //
    // -- начинаем с n = 1 -- //
    asm __volatile__ (
            R"(
            mov ecx, %[n]   # это количество итераций для цикла
         cycle:
            fld %[slag_koef]    # загрузили slag_koef в стек
            fld %[cneg1]        # загрузили значение -1 в стек
            fmulp st(1), st(0)  # умножили slag_koef на -1, с удалением -1 из стека
            fstp %[slag_koef]   # сохранили в переменную

            fld %[slag_chis]    # загрузили slag_chis в стек
            fld %[x]            # загрузили x в стек
            fmul st(1), st(0)   # умножаем slag_chis на x
            fmulp st(1), st(0)  # умножаем slag_chis на x, в этот раз с удалением x из стека
            fstp %[slag_chis]   # сохранили в переменную

            fld %[slag_znam]    # загрузили slag_znam в стек
            fld %[i]            # загружаем i - номер шага в стек
            fld %[c2]           # загружаем значение 2
            fmulp st(1), st(0)  # умножаем i на 2, с удалением 2 из стека
            fmul st(1), st(0)   # умножаем 2i на slag_znam
            fld1                # загружаем 1
            fsubp st(1), st(0)  # вычитаем из 2i единицу и получаем 2i-1
            fmulp st(1), st(0)  # умножаем slag_znam на 2i-1, удаляя из стека 2i-1
            fstp %[slag_znam]   # сохранили в переменную

            fld %[result]       # загрузили result в стек
            fld %[slag_chis]    # загрузили slag_chis в стек
            fld %[slag_znam]    # загрузили slag_znam в стек
            fdivp st(1), st(0)  # делим числитель на знаменатель, удаляя знаменатель из стека
            fld %[slag_koef]    # загружаем коэф
            fmulp st(1), st(0)  # умножаем дробь на коэф, удаляя коэф. из стека
            faddp st(1), st(0)  # складываем полученную дробь с result, удаляя дробь из стека
            fstp %[result]      # сохранили в переменную

            fld %[i]
            fld1
            faddp st(1), st(0)
            fstp %[i]

            loop cycle
            )"
            : [result] "+m" (result), [i] "+m" (i), [slag_koef] "+m" (slag_koef), [slag_chis] "+m" (slag_chis), [slag_znam] "+m" (slag_znam)
            : [n] "m" (n), [x] "m" (x), [cneg1] "m" (cneg1), [c2] "m" (c2)
        );
    return result;
}

double asm_func(double eps, double x)
{
    const double cneg1 = -1.0; // -- это для коэф -- //
    const double c2 = 2.0;

    double i = 1.0; // -- текущий шаг -- //
    double result = 1;  // -- переменная - сумма ряда при данном x и n; значение 1 здесь при n = 0 -- //
    double slag_koef = 1;  // -- коэф. текущего члена суммы -- //
    double slag_chis = 1;  // -- числитель текущего члена суммы -- //
    double slag_znam = 1;  // -- знаменатель -- //
    double slag = 1;     // -- текущий член суммы ряда -- //
    // -- начинаем с n = 1 -- //
    asm __volatile__ (
            R"(
         cycle2:
            fld %[slag_koef]    # загрузили slag_koef в стек
            fld %[cneg1]        # загрузили значение -1 в стек
            fmulp st(1), st(0)  # умножили slag_koef на -1, с удалением -1 из стека
            fstp %[slag_koef]   # сохранили в переменную

            fld %[slag_chis]    # загрузили slag_chis в стек
            fld %[x]            # загрузили x в стек
            fmul st(1), st(0)   # умножаем slag_chis на x
            fmulp st(1), st(0)  # умножаем slag_chis на x, в этот раз с удалением x из стека
            fstp %[slag_chis]   # сохранили в переменную

            fld %[slag_znam]    # загрузили slag_znam в стек
            fld %[i]            # загружаем i - номер шага в стек
            fld %[c2]           # загружаем значение 2
            fmulp st(1), st(0)  # умножаем i на 2, с удалением 2 из стека
            fmul st(1), st(0)   # умножаем 2i на slag_znam
            fld1                # загружаем 1
            fsubp st(1), st(0)  # вычитаем из 2i единицу и получаем 2i-1
            fmulp st(1), st(0)  # умножаем slag_znam на 2i-1, удаляя из стека 2i-1
            fstp %[slag_znam]   # сохранили в переменную

            fld %[result]       # загрузили result в стек
            fld %[slag_chis]    # загрузили slag_chis в стек
            fld %[slag_znam]    # загрузили slag_znam в стек
            fdivp st(1), st(0)  # делим числитель на знаменатель, удаляя знаменатель из стека
            fld %[slag_koef]    # загружаем коэф
            fmulp st(1), st(0)  # умножаем дробь на коэф, удаляя коэф. из стека
            fst %[slag]         # сохраним член суммы ряда в slag
            faddp st(1), st(0)  # складываем полученную дробь с result, удаляя дробь из стека
            fstp %[result]      # сохранили в переменную

            # увеличиваю номер шага
            fld %[i]
            fld1
            faddp st(1), st(0)
            fstp %[i]

            # организовываю работу цикла
            fld %[eps]
            fld %[slag]
            fabs
            fcomip st(0), st(1) # сравнение slag и eps, с выталкиванием slag из стека;
            fstp st(0)          # убираю из стека eps
            ja cycle2           # если slag > eps, то продолжаем работу цикла
            )"
            : [result] "+m" (result), [i] "+m" (i), [slag_koef] "+m" (slag_koef), [slag_chis] "+m" (slag_chis), [slag_znam] "+m" (slag_znam), [slag] "+m" (slag)
            : [x] "m" (x), [cneg1] "m" (cneg1), [c2] "m" (c2), [eps] "m" (eps)
        );
    return result;
}

double cpp_func(int n, double x) // -- рассчитывает сумму ряда для заданного x при заданном n -- //
{
    // -- общий вид для члена суммы ряда (-1)^n * (x^2n)/(2n)! -- //
    double result = 1;  // -- переменная - сумма ряда при данном x и n; значение 1 здесь при n = 0 -- //
    double slag_koef = 1;  // -- коэф. текущего члена суммы -- //
    double slag_chis = 1;  // -- числитель текущего члена суммы -- //
    double slag_znam = 1;  // -- знаменатель -- //
    double slag;  // -- текущий член суммы при n = i -- //
    // -- начинаем с n = 1 -- //
    for (int i = 1; i <= n; ++i)
    {
        slag_koef *= -1;
        slag_chis *= (x * x);
        slag_znam *= (2*i-1) * (2*i);
        slag = slag_koef * (slag_chis/slag_znam);
        result += slag;
    }
    return result;
}

double cpp_func(double eps, double x) // -- рассчитывает сумму ряда для заданного x при заданном eps -- //
{
    // -- общий вид для члена суммы ряда (-1)^n * (x^2n)/(2n)! -- //
    double result = 1;  // -- переменная - сумма ряда при данном x и n; значение 1 здесь при n = 0 -- //
    double slag_koef = 1;  // -- коэф. текущего члена суммы -- //
    double slag_chis = 1;  // -- числитель текущего члена суммы -- //
    double slag_znam = 1;  // -- знаменатель -- //
    double slag = 1;  // -- текущий член суммы при n = i -- //
    // -- начинаем с n = 1 -- //
    for (int i = 1; fabs(slag) > eps; ++i)
    {
        slag_koef *= -1;
        slag_chis *= (x * x);
        slag_znam *= (2*i-1) * (2*i);
        slag = slag_koef * (slag_chis/slag_znam);
        result += slag;
    }
    return result;
}

void output_result(int n, double eps, double x, double h)
{
    // -- заголовок -- //
    cout << "N n/n" << "\t" << "x" << "\t" << "Sn" << "\t\t" << "Se" << "\t\t" << "S" << endl; // заголовок таблицы
    // -- //
    for (int i = 0; i <= 10; ++i)
    {
        cout << i << "\t" << x << "\t" << cpp_func(n,x) << "\t" << cpp_func(eps,x) << "\t" << cos(x) << endl;
        // -- проверяем ассертами -- //
        assert(fabs(cpp_func(n,x) - cos(x)) <= eps);
        assert(fabs(cpp_func(eps,x) - cos(x)) <= eps);

        x += h;
    }
}

void output_result_asm(int n, double eps, double x, double h)
{
    // -- заголовок -- //
    cout << "N n/n" << "\t" << "x" << "\t" << "Sn" << "\t\t" << "Se" << "\t\t" << "S" << endl; // заголовок таблицы
    // -- //
    for (int i = 0; i <= 10; ++i)
    {
        cout << i << "\t" << x << "\t" << asm_func(n,x) << "\t" << asm_func(eps,x) << "\t" << cos(x) << endl;
        // -- проверяем ассертами -- //
        assert(fabs(asm_func(n,x) - cos(x)) <= eps);
        assert(fabs(asm_func(eps,x) - cos(x)) <= eps);

        x += h;
    }
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №7. Задание 2. Вариант 7.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    const double eps = 0.000001; // -- точность сравнения при ассерте и для самого задания -- //
    // -- входные данные для 2 задания -- //
    int n = 10;
    double a = 0.1;
    double b = 1;
    double h = (b-a) / 10;
    // -- вывод результатов -- //
    cout << "Результат C++ функции: " << endl;
    output_result(n,eps,a,h);
    cout << "\nРезультат ASM функции: " << endl;
    output_result_asm(n,eps,a,h);

    return 0;
}
