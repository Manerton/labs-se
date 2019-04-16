#include <iostream>
#include <cmath>

using namespace std;

struct point {double x; double y;};

double z(double x, double y)
{
    return x*x + x*y + y*y - 2*x - y; // (1;0)
   // return -x*x+6*x-16*y+((y*y*y)/3); // миниума нет
    //return (x-3)*(x-3)+(y-4)*(y-4); // 3 и 4
    //return x*x+2*(y*y*y)-6*x*y;
}

bool point_is_equal (point a, point b)
{
    if ((a.x == b.x) && (a.y == b.y)) return true;
    return false;
}

point isled_poisk (point b2, double h)
{

    double value_b2 = z(b2.x, b2.y); // Вычисляем значение в базисной точке

    if ( z((b2.x+h),b2.y) < value_b2 )
    {
        b2.x += h;
        value_b2 = z(b2.x, b2.y);
    }
    else if ( z((b2.x-h),b2.y) < value_b2 )
    {
        b2.x -= h;
        value_b2 = z(b2.x, b2.y);
    }
    else
    {
        if ( z(b2.x,(b2.y+h)) < value_b2 )
        {
            b2.y += h;
            value_b2 = z(b2.x, b2.y);
        }
        else if ( z(b2.x,(b2.y-h)) < value_b2 )
        {
            b2.y -= h;
            value_b2 = z(b2.x, b2.y);
        }
    }

    return b2;
}

void okruglenie (double &chislo, double e)
{
    if (fabs(chislo - static_cast<int>(chislo)) < e) chislo = static_cast<int>(chislo);
}
int main()
{
    system("chcp 1251 > nul");
    point b1, b2;
    double h,e;
    cout << "Задайте координаты базисной точки." << endl;
    cout << "Для x: ";
    cin >> b1.x;
    cout << "Для y: ";
    cin >> b1.y;
    cout << "Задайте шаг h: ";
    cin >> h;
    cout << "Задайте точность eps: ";
    cin >> e;
    b2 = b1;

    cout << "Базисная точка (" << b1.x << ";" << b1.y << ")" << endl;
    cout << "Значение в базисной точке: " << z(b1.x,b1.y) << endl;
    unsigned int i;
    for (i = 1; h > e; i++)
    {
        //Информация о шаге
        cout << "Шаг " << i << "." << endl;
        cout << "Текущая базисная точка (" << b2.x << ";" << b2.y << ")" << endl;
        cout << "Значение в этой точке: " << z(b2.x,b2.y) << endl;

        /// Исследующий поиск
        b2 = isled_poisk(b2,h);
        /// Конец исследования
         cout << "после исследования (" << b2.x << ";" << b2.y << ")" << endl;
         cout << "Значение в этой точке: " << z(b2.x,b2.y) << endl;
        // Если текущее значение больше чем значение в базисной точке
        // Уменьшаем длину шага и повторяем исследование
        if (point_is_equal(b1,b2))
        {
            h /= 10;
        }

        ///Делаем 1 шаг поиска по образцу
        else
        {
            bool poisk = true;
            for (unsigned int i_poisk = 0; poisk == true; i_poisk++)
            {
                point P;
                P.x = b1.x + 2 * (b2.x-b1.x);
                P.y = b1.y + 2 * (b2.y-b1.y);
                cout << "Поиск по образцу (" << P.x << "; " << P.y << ")" << endl;
                /// Исследующий поиск
                P = isled_poisk(P,h);
                /// Конец исследования
                cout << "Поиск по образцу после исследования (" << P.x << "; " << P.y << ")" << endl;
                cout << "Значение в этой точке: " << z(P.x,P.y) << endl;
                /// Если значение в новой временной базисной точке P > чем в предыдущей базисной точке b2, то возвращаемся к прошлой точке b2
                if (z(P.x,P.y) >= z(b2.x,b2.y) )
                {
                    b1 = b2;
                    poisk = false;

                }
                /// Иначе берем временную базисную точку P за новую базисную точку b2
                else
                {
                    b1 = b2;
                    b2 = P;
                }
                if (i_poisk == 1000)
                {
                    int menu;
                    cout << "Было произведено 1000 шагов поиска по образцу." << endl;
                    cout << "Возможно функция не имеет минимума." << endl;
                    cout << "Текущая базисная точка (" << b2.x << ";" << b2.y << ")" << endl;
                    cout << "Значение в этой точке:" << z(b2.x,b2.y) << endl;
                    cout << "Хотите продолжить работу алгоритма?" << endl;
                    cout << "1. Да." << endl;
                    cout << "2. Завершить работу программы." << endl;
                    cout << "Ввод: ";
                    cin >> menu;
                    while (menu < 1 || menu > 2)
                    {
                        cout << "Ошибка! Повторите ввод: ";
                        cin >> menu;
                    }
                    switch (menu)
                    {
                        case 1:
                        {
                            i_poisk = 0;
                            break;
                        }
                        case 2:
                        {
                            poisk = false;
                            h = e;
                            break;
                        }
                    }
                }
            }
        }
    }
    cout << "Работа алгоритма завершена!" << endl;
    cout << "Шагов итерации алгоритма: " << i-1 << endl;
    okruglenie(b2.x,e);
    okruglenie(b2.y,e);
    cout << "Базисная точка (" << b2.x << ";" << b2.y << ")" << endl;
    double value_output = z(b2.x,b2.y);
    okruglenie(value_output,e);
    cout << "Значение:" << value_output;
    return 0;
}
