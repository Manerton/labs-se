#include <iostream>
#include <ctime>
#include <set>
#include <algorithm>
#include <string>

#define cpu 1
#define motherboard 2
#define gpu 3
#define ram 4
#define powersupply 5
#define casepc 6
#define hdd 7
#define ssd 8
#define monitor 9
#define notebook 10


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

set_type Create_Market_Set (string MarketName)
{
    int N;
    cout << "Введите количество товаров в магазине " << MarketName << endl;
    cout << "Ввод: ";
    cin >> N;
    while (N < 0 || N > 10)
    {
        cout << "error!" << endl;
        cout << "Ввод: ";
        cin >> N;
    }
    set_type S;
    cout << "Перечислите товары, имеющиеся в этом магазине: " << endl;
    int tovar;
    for (int i = 0; i < N; i++)
    {
        cout << "Ввод: ";
        cin >> tovar;
        while (tovar < 1 || tovar > 10)
        {
            cout << "error!" << endl;
            cout << "Ввод: ";
            cin >> tovar;
        }
        S.insert(tovar);
    }
    return S;
}

void display_menu ()
{
    cout << "*** Меню ***" << endl;
    cout << "1) В четырех магазинах продают некоторые виды товаров из имеющегося списка. Определить, какие товары продаются в каждом магазине, какие товары продают только в одном магазине, а какие товары есть хотя бы в двух магазинах." << endl;
    cout << "2) Дан текст из строчных латинских букв, за которыми следует точка. Напечатать все буквы, входящие в текст не менее двух раз; все согласные буквы, входящие только в одно слово.." << endl;
    cout << "3) Разгадайте ребус: БАРБОС + БОБИК = СОБАКИ (два решения)." << endl;
    cout << "4) Завершить работу программы." << endl;
}

set_type tovari_in_each_market (set_type &M1, set_type &M2, set_type &M3, set_type &M4)
{
    set_type M12;
    set_type M34;
    set_type result;
    set_intersection (M1.begin(), M1.end(), M2.begin(), M2.end(), inserter(M12, M12.begin()));
    set_intersection (M3.begin(), M3.end(), M4.begin(), M4.end(), inserter(M34, M34.begin()));
    set_intersection (M12.begin(), M12.end(), M34.begin(), M34.end(), inserter(result, result.begin()));

    if (result.size() != 0)
    {
        set_type::iterator it;
        for(it = result.begin(); it != result.end(); it++)
        {
            M1.erase(*it);
            M2.erase(*it);
            M3.erase(*it);
            M4.erase(*it);
        }
    }
    return result;
}

set_type at_least_in_two_market (set_type &M1, set_type &M2, set_type &M3, set_type &M4)
{
    set_type result;
    set_intersection (M1.begin(), M1.end(), M2.begin(), M2.end(), inserter(result, result.begin()));
    set_intersection (M1.begin(), M1.end(), M3.begin(), M3.end(), inserter(result, result.begin()));
    set_intersection (M1.begin(), M1.end(), M4.begin(), M4.end(), inserter(result, result.begin()));
    set_intersection (M2.begin(), M2.end(), M3.begin(), M3.end(), inserter(result, result.begin()));
    set_intersection (M2.begin(), M2.end(), M4.begin(), M4.end(), inserter(result, result.begin()));
    set_intersection (M3.begin(), M3.end(), M4.begin(), M4.end(), inserter(result, result.begin()));


    if (result.size() != 0)
    {
        set_type::iterator it;
        for(it = result.begin(); it != result.end(); it++)
        {
            if (M1.find(*it) != M1.end()) M1.erase(*it);
            if (M2.find(*it) != M1.end()) M2.erase(*it);
            if (M3.find(*it) != M1.end()) M3.erase(*it);
            if (M4.find(*it) != M1.end()) M4.erase(*it);
        }
    }
    return result;
}

void display_set_market(set_type &S)
{
    set_type::iterator it;
    for(it = S.begin(); it != S.end(); it++)
    {
        int tovar = *it;
        switch (tovar)
        {
            case cpu: cout <<"- процессоры "; break;
            case motherboard: cout <<"- материнские платы "; break;
            case gpu: cout <<"- видеокарты "; break;
            case ram: cout <<"- оперативная память "; break;
            case powersupply: cout <<"- блоки питания "; break;
            case casepc: cout <<"- корпуса "; break;
            case hdd: cout <<"- жесткие диски "; break;
            case ssd: cout <<"- SSD диски "; break;
            case monitor: cout <<"- мониторы "; break;
            case notebook: cout <<"- ноутбуки "; break;

        }
    }
}

void display_set_char (set<char> &S)
{
    set<char>::iterator it;
    for(it = S.begin(); it != S.end(); it++)
    {
        cout << *it;
    }
}

void ex1 ()
{
    set_type M1 = Create_Market_Set("ДНС");
    set_type M2 = Create_Market_Set("Ситилинк");
    set_type M3 = Create_Market_Set("Мвидео");
    set_type M4 = Create_Market_Set("Регард");
    set_type everymarket = tovari_in_each_market(M1, M2, M3, M4);
    if (everymarket.size() != 0)
    {
        cout << "Товары, которые продаются в каждом магазине: ";
        display_set_market(everymarket);
        cout << endl;
    }
    set_type two_market_set = at_least_in_two_market(M1,M2,M3,M4);
    if (two_market_set.size() != 0)
    {
        cout << "Товары, которые есть хотя бы в двух магазинах: ";
        display_set_market(two_market_set);
        cout << endl;
    }
    if (M1.size() != 0)
    {
        cout << "Товары, которые продают только в ДНС: ";
        display_set_market(M1);
        cout << endl;
    }
    if (M2.size() != 0)
    {
        cout << "Товары, которые продают только в Ситилинк: ";
        display_set_market(M2);
        cout << endl;
    }
    if (M3.size() != 0)
    {
        cout << "Товары, которые продают только в Мвидео: ";
        display_set_market(M3);
        cout << endl;
    }
    if (M4.size() != 0)
    {
        cout << "Товары, которые продают только в Регард: ";
        display_set_market(M4);
        cout << endl;
    }

    system("pause");
}


void string_to_multiset (const string &S, multiset<char> &text)
{
    int N = S.size();
    for (int i = 0; i < N; i++)
    {
        text.insert(S[i]);
    }
}

void string_to_set (const string &S, set<char> &text)
{
    int N = S.size();
    for (int i = 0; i < N; i++)
    {
        text.insert(S[i]);
    }
}

set<char> soglasnie_bukvi (const set<char> &S) //
{
    set<char> result;
    string soglasnie = "bcdfghklmnpqrstvxz";
    for (char c : S)
        if (soglasnie.find(c) != string::npos)
        {
            result.insert(c);
        }
    return result;
}


void ex2 ()
{
    string S;
    multiset<char> text;
    set<char> text_bez_povtorov;
    cout << "Введите текст на английском языке (строчными буквами)." << endl;
    cin.get();
    getline(cin,S);
    string_to_multiset(S,text);
    string_to_set(S, text_bez_povtorov);
    set<char> bukvi_ne_menee_dvuh_raz;
    set_difference(text.begin(), text.end(), text_bez_povtorov.begin(), text_bez_povtorov.end(), inserter(bukvi_ne_menee_dvuh_raz, bukvi_ne_menee_dvuh_raz.begin()));
    cout << "Напечатать все буквы, входящие в текст не менее двух раз:" << endl;
    display_set_char(bukvi_ne_menee_dvuh_raz);
    set<char> text_bez_bukvi_ne_menee_dvuh_raz;
    set_difference(text_bez_povtorov.begin(), text_bez_povtorov.end(), bukvi_ne_menee_dvuh_raz.begin(), bukvi_ne_menee_dvuh_raz.end(), inserter(text_bez_bukvi_ne_menee_dvuh_raz, text_bez_bukvi_ne_menee_dvuh_raz.begin()));
    cout << "\nВсе согласные буквы, входящие только в одно слово: " << endl;
    set<char> result = soglasnie_bukvi(text_bez_bukvi_ne_menee_dvuh_raz);
    display_set_char(result);
    cout << endl;

}

void ex3 ()
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

int main()
{
    srand(time(0));
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
            case 1:
                cout << "Имеется 4 магазина: ДНС, Ситилинк, Мвидео и Регард." << endl;
                cout << "А так же список видов товаров: " << endl;
                cout << "1. Процессоры" << endl;
                cout << "2. Материнские платы" << endl;
                cout << "3. Видеокарты" << endl;
                cout << "4. Оперативная память" << endl;
                cout << "5. Блоки питания" << endl;
                cout << "6. Корпуса" << endl;
                cout << "7. Жесткие диски" << endl;
                cout << "8. Твердотельные диски SSD" << endl;
                cout << "9. Мониторы" << endl;
                cout << "10. Ноутбуки" << endl;
                cout << "Необходимо определить, какие товары продаются в каждом магазине, какие товары продают только в одном магазине, а какие товары есть хотя бы в двух магазинах." << endl;
                ex1();
                break;
            case 2:
                ex2();
                break;
            case 3:
                cout << "Разгадайте ребус: БАРБОС + БОБИК = СОБАКИ (два решения)." << endl;
                ex3();
                break;
            }
        }
    }


    return 0;
}
