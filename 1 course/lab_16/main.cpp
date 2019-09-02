#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

using Pfun = double (*) (const double);  // указатель на функцию

using Lfun = double (&) (const double);  // ссылка на функцию

const double eps = 1e-4; // точность

double fun_1( double x )
{
    return x;
}

double fun_2( double x )
{
    return 2*x - x*x;
}

double fun_3( double x )
{
    if (x == 0) throw "деление на ноль.";
    return 1/x;
}

double fun_4( double x )
{
    return sqrt(fabs(x));
}

double find_volume (Pfun fun, double Xn, double Xk, double h) // с помощью указателя
{
    double result = 0;
    double half_h = h/2;

    for ( double x = Xn; x <= Xk; x += h )
    {
        double fun_result = fun(x + half_h) ;
        result += fun_result * fun_result; // по формуле объема используется квадрат исходной функции
    }
    result *= h;
    return result;
}

double integral (Pfun fun, double Xn, double Xk) // с помощью указателя
{
    double result, prev_result, h, delta;
    int n = 5;
    h = (Xk-Xn) / n; // делим фигуру на 5 прямоугольников
    result = find_volume(fun, Xn, Xk, h); // находим площадь этих фигур методом средних прямоугольников

    do {
        n *= 2;
        prev_result = result;
        h = fabs(Xk-Xn) / n; // делим фигуру уже на 10 прямоугольников
        result = find_volume(fun, Xn, Xk, h); // находим площадь этих фигур методом средних прямоугольников
        delta = fabs(result - prev_result) / 3; // правило Рунге
    } while (delta > eps);

    return result;
}

double find_volume_2 (Lfun fun, double Xn, double Xk, double h) // с помощью ссылки
{
    double result = 0;
    double half_h = h/2;

    for ( double x = Xn; x <= Xk; x += h )
    {
        double fun_result = fun(x + half_h) ;
        result += fun_result * fun_result; // по формуле объема используется квадрат исходной функции
    }
    result *= h;
    return result;
}

double integral_2 (Lfun fun, double Xn, double Xk) // с помощью ссылки
{
    double result, prev_result, h, delta;
    int n = 5;
    h = (Xk-Xn) / n; // делим фигуру на 5 прямоугольников
    result = find_volume_2(fun, Xn, Xk, h); // находим площадь этих фигур методом средних прямоугольников

    do {
        n *= 2;
        prev_result = result;
        h = fabs(Xk-Xn) / n; // делим фигуру уже на 10 прямоугольников
        result = find_volume_2(fun, Xn, Xk, h); // находим площадь этих фигур методом средних прямоугольников
        delta = fabs(result - prev_result) / 3; // правило Рунге
    } while (delta > eps);

    return result;
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №16. Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
    Pfun Arr_fun[] {fun_1, fun_2, fun_3, fun_4}; // массив указателей на функции, или, грубо говоря, массив функций
                                                 // так как лень делать свитч кейсы
    int nomer;
    cout << "Найти объём тела вращения методом прямоугольников с заданной точностью 1e-4." << endl;
    cout << "1. Функция x" << endl;
    cout << "2. Функция (2x - x^2)" << endl;
    cout << "3. Функция (1/x)" << endl;
    cout << "4. Функция sqrt(x)" << endl;
    cout << "Выберите функцию: ";
    cin >> nomer;
    while (nomer < 0 || nomer > 4)
    {
        cout << "Ошибка!" << endl;
        cout << "Выберите функцию: ";
        cin >> nomer;
    }
    double Xn, Xk;
    cout << "Введите диапазон (левую и правую границу интегрирования): ";
    cin >> Xn >> Xk;
    cout << "Осуществляется проверка границ интегрирования..." << endl;
    try // ищем исключения, которые выбрасываются в блоке try и отправляем их для обработки в блок(и) catch
    {
        Arr_fun[nomer-1](Xn);
        Arr_fun[nomer-1](Xk);
        cout << "Функция существует на границах интегрирования." << endl;
        cout << "Переходим к поиску объема тела." << endl;
        double result_pointer, result_link;
        if (Xn < Xk)
        {
            result_pointer = integral(Arr_fun[nomer-1], Xn, Xk); // ищу объем с помощью функции зависящей от указателя
            result_link = integral_2(*Arr_fun[nomer-1], Xn, Xk); // ищу объем с помощью функции зависящей от ссылки, для этого разыменовываю указатель на функцию и передаю саму функцию
        } else {
            result_pointer = -integral(Arr_fun[nomer-1], Xk, Xn);
            result_link = -integral_2(*Arr_fun[nomer-1], Xk, Xn);
        }
        cout << "(С помощью указателя) Объем равен " << fixed <<setprecision(4) << result_pointer << " * Pi" << endl;
        cout << "(С помощью ссылки) Объем равен " << fixed <<setprecision(4) << result_link << " * Pi" << endl;
    }
    catch (const char* exception) // обработка исключений типа const char*
    {
        cerr << "Ошибка: " << exception << endl;
    }
    return 0;
}
