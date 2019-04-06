#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

void input(int &n)
{
    cout << "Введите значение n (от 1 до 10) -> ";
    cin >> n;
    while (n <= 0 || n > 10)
    {
        cout << "Некорректный ввод." << endl;
        cout << "Введите значение n -> ";
        cin >> n;
    }
}

int find_nod (int a, int b) {
    if (a < 0) a = -a;
    if (b == 0) return a;
    return find_nod(b, a%b);
}

void sokr_drob (int &chis, int &znam)
{
    int nod = find_nod(chis, znam);
    if (nod > 1)
    {
        chis /= nod;
        znam /= nod;
    }
}

// Если нужно вывести только на экран - то третий параметр - false, если на экран и в файл - file = true;
void display_drob (int chis, int znam, bool file)
{
    bool znak_minus = false;
    int defis_chis(1), defis_znam(1);
    int count_of_defis;
    int chis_temp = chis;
    int znam_temp = znam;
    if (chis < 0)
    {
        znak_minus = true;
    }
    while (chis_temp / 10)
    {
        defis_chis++;
        chis_temp /= 10;
    }
    while (znam_temp / 10)
    {
        defis_znam++;
        znam_temp /= 10;
    }
    if (defis_chis >= defis_znam) count_of_defis = defis_chis;
    else if (defis_chis < defis_znam) count_of_defis = defis_znam;

    if (znak_minus && (defis_chis == defis_znam))
    {
        cout << " " << chis << endl;
        cout << "  " << setfill('-') << setw(++count_of_defis) << "\n";
        cout << "  " << znam << endl;
    } else {
        cout << " " << chis << endl;
        cout << " " << setfill('-') << setw(++count_of_defis) << "\n";
        cout << " " << znam << endl;
    }

    if (file)
    {
        ofstream out("Out.txt");
        out << " " << chis << endl;
        out << " " << setfill('-') << setw(count_of_defis) << "\n";
        out << " " << znam << endl;
        out.close();
    }
}

void func (int n, int &chis, int &znam)
{
    int slag_chis, slag_znam;
    //вычисляю сумму первых двух слагаемых как сумму обычных дробей
    chis = 1;
    znam = 3;
    slag_chis = -2;
    slag_znam = 4;
    chis = (chis * slag_znam) + (znam * slag_chis);
    znam = znam * slag_znam;
    //полученную дробь сокращщаю
    sokr_drob(chis, znam);
    cout << "Сумма первых двух слагаемых: " << endl;
    display_drob(chis, znam, false);

    slag_chis = 3; // слагаемое при n = 2
    slag_znam = 5;

    for (int i = 2; i <= n; i++) // до тех пор, пока не вычислена вся сумма, прибавляю к имеющейся дроби очередное слагаемое и при необходимости сокращаю
    {
        if (i % 2 == 0)
        {
            chis = (chis * slag_znam) - (znam * slag_chis);
        } else {
            chis = (chis * slag_znam) + (znam * slag_chis);
        }

        znam = znam * slag_znam;
        sokr_drob(chis, znam);
        cout << "Сумма дробей при n = " << i << endl;
        display_drob(chis, znam, false);
        slag_chis += 2;
        slag_znam += 2;
    }
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №4 Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
    cout << "Способ 1. Числитель и знаменатель - две отдельные переменные." << endl << endl;
    int n;
    input(n);
    // тестовые данные
    // при n = -1 и 0 - будет запрошен повторный ввод
    // при n = 1 -> -1/6
    // при n = 2 -> -23/30
    // при n = 3 -> -11/210
    // при n = 4 -> -523/630
    // при n = 5 -> -83/6930
    // при n = 6 -> -77309/90090

    int chis, znam;
    func(n, chis, znam);
    cout << "Результирующие данные: " << endl;
    display_drob(chis, znam, true);

    return 0;
}
