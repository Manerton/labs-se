#include <iostream>
#include <cmath>

using namespace std;

double logab(double a, double b)
{
    return log(b) / log(a);
}

double f(double x, double a, double b, double c)
{
    if (c <= 0 || c == 1) throw "введеное основание логарифма (c) должно быть больше 0 и не равно 1.";
    if (x <= 0) throw "взят отрезок, на котором функция не существует (логарифм не существует).";
    double logab_result = logab(c,x);
    if (logab_result < 0) throw "взят отрезок, на котором функция не существует (подкоренное выражение отрицательное).";
    return (sqrt(logab_result) * (a*x + b) - 1);
}
double Root(double xn, double xk, double a, double b, double c, double eps)
{
    if (eps <= 0) throw "введена отрицательная или нулевая точность.";
    double f_xn = f(xn,a,b,c);
    double f_xk = f(xk,a,b,c);
    if ((f_xn * f_xk) > 0) throw "взят отрезок, на котором метод неприменим (функция на левой и на правой границах отрезка имеет одинаковые знаки).";
    double xm = (xn+xk)/2;
    if (fabs(xk-xn) < eps) return xm;
    if (f_xn * f(xm,a,b,c) <= 0)
    {
        xk = xm;
    } else {
        xn = xm;
    }
    return Root(xn,xk,a,b,c,eps);
}


int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №15. Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
    double a, b, c, xn, xk, eps;
    cout << "Введите значение a: ";
    cin >> a;
    cout << "Введите значение b: ";
    cin >> b;
    cout << "Введите значение c: ";
    cin >> c;
    cout << "Введите левую границу X_n: ";
    cin >> xn;
    cout << "Введите правую границу X_n: ";
    cin >> xk;
    cout << "Введите точность eps: ";
    cin >> eps;

    try // ищем исключения, которые выбрасываются в блоке try и отправляем их для обработки в блок(и) catch
    {
        double result = Root(xn,xk,a,b,c,eps);
        cout << "Корень функции: " << result;
    }
    catch (const char* exception) // обработка исключений типа const char*
    {
        cerr << "Ошибка: " << exception << endl;
    }


    return 0;
}
