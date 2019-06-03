#include <iostream>
#include <set>
#include <algorithm>

using namespace std;

typedef set<int, less<int> > set_type;

set_type Set_from_Number(int Number)
{
    set_type S;
    while (Number>0)
    {
        S.insert(Number % 10);
        Number /= 10;
    }
    return S;
}

void display_menu ()
{
    cout << endl;
    cout << "*** Меню ***" << endl;
    cout << "1) В четырех магазинах продают некоторые виды товаров из имеющегося списка. Определить, какие товары продаются в каждом магазине, какие товары продают только в одном магазине, а какие товары есть хотя бы в двух магазинах." << endl;
    cout << "2) Дан текст из строчных латинских букв, за которыми следует точка. Напечатать все буквы, входящие в текст не менее двух раз; все согласные буквы, входящие только в одно слово.." << endl;
    cout << "3) Разгадайте ребус: БАРБОС + БОБИК = СОБАКИ (два решения)." << endl;
    cout << "4) Завершить работу программы." << endl;
}

int main()
{
    system("chcp 1251 > nul");
    int menu(0);
    while (menu != 4)
    {
        display_menu();
        cout << "Выберите пункт меню: ";
        cin >> menu; // выбираем пункт меню и запоминаем номер
        //проверка на дурака
        while (menu < 1 || menu > 4)
        {
            cout << "error!" << endl;
            cout << "Выберите пункт меню: ";
            cin >> menu;
        }
        if (menu != 4)
        {
            switch (menu) // переходим непосредственно к выполнению заданий
            {
            case 3:
            {
                set_type M;
                set_type N;
                set_type MN;
                for (int i = 100000; i < 900001; i++) // барбос
                {
                    int i1 = i/100000; // первая буква б
                    int i4 = (i/100)%10; // четвертая буква б
                    int i6 = i%10; // последняя буква С

                    if (i6 != 0 && i1 != 9 && (i1+1 == i6) && i1 == i4) // если буква с - не равна нулю, б != 9, б+1 = с
                    {
                        M = Set_from_Number(i);
                        if (M.size() == 5) // проверка на неповторяющиеся цифры
                        {
                            int i2 = (i/10000)%10; // вторая буква а

                            int i5 = (i/10)%10; // пятая буква о
                            for (int j=10000; j<100000; j++) // бобик
                            {

                                int j1 = j/10000; // первая буква - б
                                if (i2 + j1 > 9 && i1 == j1)
                                {

                                    N = Set_from_Number(j);
                                    MN.clear();
                                    set_intersection (M.begin(), M.end(), N.begin(), N.end(), inserter(MN, MN.begin())); // только два общих элемента - буква Б и О

                                    if (N.size() == 4 && MN.size() == 2) // проверка на неповторяющиеся цифры
                                    {

                                        int j2 = (j/1000)%10; // вторая буква - о
                                        int j3 = (j/100)%10; // третья буква - б
                                        if (j1 == j3 && i5 == j2) // проверка на барбос и бобик
                                        {
                                            int j4 = (j/10)%10; // четвертая буква - и
                                            int j5 = j%10; // последняя буква - к

                                            int ij = i + j; // собаки
                                            if (ij / 100000 == i6)
                                            {
                                                if (ij == i6*100000 + j2*10000 + i1*1000 + i2 * 100 + j5 * 10 + j4)
                                                {
                                                    MN = Set_from_Number(ij);
                                                    if (MN.size() == 6)
                                                    {
                                                        cout << i << " + " << j <<  " = " << ij << endl;
                                                    }
                                                }
                                            }


                                        }
                                    }
                                }
                            }
                        }


                    }
                }
            }
            }
        }
    }


    return 0;
}
