#include <iostream>
#include <fstream>
#include <cmath>
#include <windows.h>
#include <iomanip>

using namespace std;

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №2. Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
    // Выбор пункта меню
    int menu;
    do {
        cout << "Меню:\n" << endl;
        cout << "1. Задание 1 \n" << "2. Задание 2 \n" << "0. Выход" << endl;
        cout << "Введите номер задания для выполнения -> ";
        cin >> menu;
    } while (menu < 0 || menu > 2);
    switch (menu)
    {
    case 1: // первое задание
    {
        //-----УСЛОВИЕ ЗАДАНИЯ-----
        cout << "\n Вычислите и выведите на экран значения функции y = f(x) в точках x0=-Pi, x1=x0+h, x2=x1+h, … x12 = +Pi, где h = Pi/6." << endl;
        cout << " Иными словами: затабулируйте функцию y = f(x) на отрезке [-Pi ; Pi] с шагом h." << endl;
        cout << " y = f(x) : sin^4(a) + sin^4(x)" << endl;
        cout << "\t    -------------------" << endl;
        cout << "\t\t    a+x" << endl << endl;
        const int N = 12;               //// переменные,
        double a, x_n, x, x_k, h; //  используемые в 1 задании
        a = 0.5;
        x_n = -M_PI;
        x = x_n;
        x_k = M_PI;
        h = (x_k - x_n) / N;
        double func_1 = pow(sin(a),4); // чтобы не считать в функции одно и тоже
        double y; // наша функция
        int sposob_for_ex1;
        //-----ВЫБОР СПОСОБА-----
        do {
            cout << "1. Способ 1 - Цикл с предусловием" << endl;
            cout << "2. Способ 2 - Цикл с постусловием" << endl;
            cout << "3. Способ 3 - Цикл с параметром" << endl;
            cout << "Введите номер способа -> ";
            cin >> sposob_for_ex1;
        } while (sposob_for_ex1 < 1 || sposob_for_ex1 > 3);
        //-----ОТКРЫВАЕМ ФАЙЛ-----
        ofstream output("Out.txt");
        //-----ЗАГОЛОВОК ВЫВОДА НА ЭКРАН------
        cout << "\t" << "x" << "\t" << "|" << "\t" << "y" << endl;
        cout << setfill('-') << setw(35); // рисую тире
        cout << "-" << endl;
        cout << setfill(' '); // сбрасываю настройки setfill
        //-----ЗАГОЛОВОК ВЫВОДА В ФАЙЛ-----
        output << "\t" << "x" << "\t" << "|" << "\t" << "y" << endl;
        output << setfill('-') << setw(35); // рисую тире
        output << "-" << endl;
        output << setfill(' ');
        //-----СПОСОБЫ-----
        switch (sposob_for_ex1)
        {
        case 1:
            while (x <= x_k)
            { // цикл с предусловием
                y = (func_1 + pow(sin(x),4))/(a+x); // функция
                //Вывод на экран
                cout << fixed << setprecision(2); // перевожу в фиксированную форму вывода действительного числа, иначе не округляет до 2 цифр после запятой
                cout << "\t" << x << "\t" << "|" << "\t" << y << endl;
                //Вывод в файл
                output << fixed << setprecision(2);
                output << "\t" << x << "\t" << "|" << "\t" << y << endl;
                x += h;
            }
            break;
        case 2:
            do { // цикл с постусловием
                y = (func_1 + pow(sin(x),4))/(a+x); // функция
                //Вывод на экран
                cout << fixed << setprecision(2); // перевожу в фиксированную форму вывода действительного числа, иначе не округляет до 2 цифр после запятой
                cout << "\t" << x << "\t" << "|" << "\t" << y << endl;
                //Вывод в файл
                output << fixed << setprecision(2);
                output << "\t" << x << "\t" << "|" << "\t" << y << endl;
                x += h;
            } while (x <= x_k);
            break;
        case 3:
           for (x = x_n; x <= x_k; x += h) {
                y = (func_1 + pow(sin(x),4))/(a+x); // функция
                //Вывод на экран
                cout << fixed << setprecision(2); // перевожу в фиксированную форму вывода действительного числа, иначе не округляет до 2 цифр после запятой
                cout << "\t" << x << "\t" << "|" << "\t" << y << endl;
                //Вывод в файл
                output << fixed << setprecision(2);
                output << "\t" << x << "\t" << "|" << "\t" << y << endl;
            }
            break;
        }
        //-----КОНЕЦ СПОСОБОВ И ПЕРВОГО ЗАДАНИЯ-----
        //-----ЗАКРЫВАЕМ ФАЙЛ-----
        output.close();
        break;
    }
    case 2:
    {
        //-----УСЛОВИЕ ЗАДАНИЯ-----
        cout << "\n* Символ E - символ принадлежности множеству." << endl;
        cout << "\n Затабулировать функцию Z=f(x,y) с шагами dx и dy внутри прямоугольника x E [a,b], y E [c,d] \n и области существования данной функции." << endl << endl;
        //-----КОНЕЦ УСЛОВИЯ-----
        double a, b, c, d, x, y, dx, dy; //  используемые в 2 задании
        //-----ПРОВЕРКА ВВОДА-----
        do {
            cout << "Введите значение a -> ";
            cin >> a;
            cout << "Введите значение b -> ";
            cin >> b;
            if (fabs(a - b) < 0.001 || a > b) cout << "Некорректный ввод данных." << endl;
        } while (fabs(a - b) < 0.001 || a > b);
        do {
            cout << "Введите значение c -> ";
            cin >> c;
            cout << "Введите значение d -> ";
            cin >> d;
            if (fabs(c - d) < 0.001 || c > d) cout << "Некорректный ввод данных." << endl;
        } while (fabs(c - d) < 0.001 || c > d);
        //-----КОНЕЦ ВВОДА-----
        dx = (b - a) / 10;
        dy = (d - c) / 10;
        cout << "dx и dy были расчитаны с учетом 10 шагов." << endl;
        cout << "dx -> " << dx << endl;
        cout << "dy -> " << dy << endl << endl;
        double z; // наша функция
        int sposob_vivoda;

        //-----ВЫБОР СПОСОБА ВЫВОДА-----
        do {
            cout << "1. Способ 1 - Вывод на экран" << endl;
            cout << "2. Способ 2 - Вывод в файл" << endl;
            cout << "Введите номер способа -> ";
            cin >> sposob_vivoda;
        } while (sposob_vivoda < 1 || sposob_vivoda > 2);
        //-----КОНЕЦ ВЫБОРА-----

        //-----СПОСОБЫ ВЫВОДА И ВТОРОЕ ЗАДАНИЕ----
        switch (sposob_vivoda)
        {
        case 1:
        {
            //-----ЗАГОЛОВОК ВЫВОДА НА ЭКРАН------
            cout << setw(3) << "\\" << setw(4) << "X" << setw(2) << "|" << endl;
            cout << setw(4) << "\\" << setw(5) << "|" << endl;
            cout << setw(2) << "Y" << setw (3) << "\\" << setw(4) << "|";
            for (x = a; x <= b; x += dx) {
                cout << "\t" << x;
            }
            cout << endl;
            cout << setfill('-') << setw(113);
            cout << "\n";
            cout << setfill(' ');
            //-----КОНЕЦ ЗАГОЛОВКА-----
            //-----ВЫВОД ТАБЛИЦЫ-----
            for (y = c; y <= d; y += dy)
            {
                cout << y << "\t" << "|";
                for (x = a; x <= b; x += dx)
                {
                    if (fabs(x - y) < 0.001) cout  << "\t*";
                    else
                    {
                        z = exp(x*y/(x-y));
                        cout << "\t";
                        cout << setprecision (2) << fixed << z;
                    }
                }
               // cout.unsetf(std::ios_base::floatfield); // тестовый параметр, вроде работает на старых компиляторах вместо defaultfloat
               cout << defaultfloat << endl; // вывод "y" без лишних запятых, в нормальной форме
               cout << setfill('-') << setw(113);
               cout << "\n";
               cout << setfill(' ');
               //-----КОНЕЦ ВЫВОДА ТАБЛИЦЫ-----
            }
            break;
        }
        case 2:
        {
            ofstream output("Out.txt");
            //-----ЗАГОЛОВОК ВЫВОДА В ФАЙЛ------
            output << setw(3) << "\\" << setw(4) << "X" << setw(2) << "|" << endl;
            output << setw(4) << "\\" << setw(5) << "|" << endl;
            output << setw(2) << "Y" << setw (3) << "\\" << setw(4) << "|";
            for (x = a; x <= b; x += dx) {
                output << "\t" << x;
            }
            output << endl;
            output << setfill('-') << setw(113);
            output << "\n";
            output << setfill(' ');
            //-----КОНЕЦ ЗАГОЛОВКА-----
            //-----ВЫВОД ТАБЛИЦЫ-----
            for (y = c; y <= d; y += dy)
            {
                output << y << "\t" << "|";
                for (x = a; x <= b; x += dx)
                {
                    if (fabs(x - y) < 0.001) output  << "\t*";
                    else
                    {
                        z = exp(x*y/(x-y));
                        output << "\t";
                        output << setprecision (2) << fixed << z;
                    }
               }
               // cout.unsetf(std::ios_base::floatfield); // тестовый параметр, вроде работает на старых компиляторах вместо defaultfloat
               output << defaultfloat << endl; // вывод "y" без лишних запятых, в нормальной форме
               output << setfill('-') << setw(113);
               output << "\n";
               output << setfill(' ');
               //-----КОНЕЦ ВЫВОДА ТАБЛИЦЫ-----
            }
            output.close();
            break;
        }
        }
        //КОНЕЦ СПОСОБОВ И ВТОРОГО ЗАДАНИЯ
        cout << "Был осуществлен вывод в файл.";
        break;
    }
    return 0;
    }
}
