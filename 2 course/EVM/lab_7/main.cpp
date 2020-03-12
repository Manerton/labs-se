#include <iostream>
#include <cassert>
#include <cmath>
#include <iomanip>

using namespace std;

void asm_func(double x, double a, int N, double h, double* arr_y)
{
    const double c14 = 14.0; // -- это для знаменателя -- //
    asm __volatile__ (
            R"(
            mov rbx, 0      # это индекс массива-результата
            mov ecx, %[N]   # это количество итераций для цикла
         cycle:
            fld %[a]            # загрузили значение a в стек
            fmul st(0), st(0)   # возвели a в квадрат
            fld %[a]            # загрузили a в стек
            fmulp st(1), st(0)  # возводим a в куб, удаляя множитель a с вершины стека
            fld %[x]            # загружаем x в стек
            faddp st(1), st(0)  # складываем a^3 с x, удаляя x с вершины стека
            fsqrt
            fsin
            fld %[a]
            fld %[x]
            fmulp st(1), st(0)  # умножение a на x, с удалением x из стека
            fld %[c14]          # загружаем число 14
            faddp st(1), st(0)  # 14 + ax, с удалением 14 из стека
            fdivp st(1), st(0)  # делим

            # сохраняем результат в массив y
            fstp %[arr_y] [(rbx*8)]
            inc rbx

            # x += h
            fld %[x]
            fld %[h]
            faddp st(1), st(0)
            fstp %[x]

            loop cycle
            )"
            : [arr_y] "+m" (*arr_y), [x] "+m" (x)
            : [a] "m" (a), [c14] "m" (c14), [N] "m" (N), [h] "m" (h)
        );
}

inline void cpp_func(double x, double a, double N, double h, double* arr_y)
{
    for (int i = 0; i <= N; ++i)
    {
        double chis = sin (sqrt ( a*a*a + x));
        double znam = 14 + a*x;
        arr_y[i] = chis / znam;
        x += h;
    }
}

void output_result(double xn, double N, double h, double* arr_y)
{
    double x = xn;
    // -- заголовок -- //
    cout << "\t" << "x" << "\t" << "|" << "\t" << "y" << endl;
    cout << setfill('-') << setw(35); // рисую тире
    cout << "-" << endl;
    cout << setfill(' '); // сбрасываю настройки setfill
    // -- //
    for (int i = 0; i <= N; ++i)
    {
        cout << "\t" << x << "\t" << "|" << "\t" << arr_y[i] << endl;
        x += h;
    }
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №7. Задание 1. Вариант 7.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    const double eps = 0.00001; // -- точность сравнения при ассерте -- //
    // -- входные данные для 1 задания -- //
    int N = 12;
    double a = 1;
    double xn = 2;
    double xk = 3;
    double h = (xk - xn) / N;
    // -- массивы-результаты -- //
    double *arr_y_cpp = new double[N+1];
    double *arr_y_asm = new double[N+1];
    cpp_func(xn, a, N, h, arr_y_cpp);
    asm_func(xn, a, N+1, h, arr_y_asm);
    // -- проверка ассертом содержимого массивов -- //
    for (int i = 0; i <= N; ++i)
    {
        assert(fabs(arr_y_asm[i] - arr_y_cpp[i]) <= eps);
    }
    // -- вывод массив
    cout << "Результат C++ функции: " << endl;
    output_result(xn,N,h,arr_y_cpp);
    cout << "\nРезультат ASM функции: " << endl;
    output_result(xn,N,h,arr_y_asm);

    return 0;
}
