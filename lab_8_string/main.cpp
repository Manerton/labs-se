#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

int ex1 (string S, char c)
{
    if (S.find(c) != string::npos) // Если такой символ был найден
    {
        return S.find(c); // то вернуть его позицию
    } else return -1;
}

bool eng_word (string S) // если символы в строке все находятся в диапазоне английских букв, то true
{
    for (char c : S) if (c < 'A' || c > 'z') return false;
    return true;
}

bool secondname (string S)
{
    int N = S.size();
    if (S[0] < 'А' || S[0] > 'Я' || S.length() < 2) return false; // если первая буква нерусская и незаглавная или если длина меньше двух букв, то false
    for (int i = 1; i < N; i++)
    {
        if (S[i] < 'а' || S[i] > 'я') return false; // проверяем чтобы все буквы были русские
    }
    return true;
}

vector<string> zapolnenie_vectora ()
{
    vector<string> v;
    string S;
    cout << "Если вы хотите прекратить ввод, введите вместо слова число 1! ";
    while (S != "1")
    {
        cout << "Введите английское слово -> ";
        cin >> S;
        if (eng_word(S)) v.push_back(S);
        else if (S != "1") cout << "Слово не является английским, либо в слове присутствует лишний знак." << endl;
    }
    return v;
}

vector<string> zapolnenie_vectora (int N, int M)
{
    vector<string> v;
    string S;
    for (int i = 0; i < N; i++)
    {
        cout << "Введите фамилию студента на русском языке (с большой буквы): ";
        cin >> S;
        while (!secondname(S))
        {
            cout << "Слово не является фамилией, либо в слове присутствует лишний знак." << endl;
            cout << "Повторите ввод: ";
            cin >> S;
        }
        for (int j = 0; j < M; j++)
        {
            int random = 2 + rand() % 4; // проверяем чтобы все буквы были русские
            S += " "; // вставляю пробел
            S += to_string(random); // вставляю цифру
        }
        v.push_back(S);
    }
    return v;
}

int ex3 (const vector<string> &v, int N, int M)
{
    int result = 0;
    for (int i = 0; i < N; i++)
    {
        int count = 0;
        for (char c : v[i]) // проверяю каждый символ слова, если кол-во найденных пятерок равен M, то
        {
            if (c == '5') count++;
        }
        if (count == M) result++; // прибавляю к результату (кол-во отличников) единицу
    }
    return result;
}

void sort_vector (vector<string> &v)
{
    int N = v.size();
    for (int i = 0; i != N-1; i++)
    {
        for (int j = i+1; j != N; j++)
        {
           char from_string_i = (char)tolower(v[i][0]); // понижаю первую букву, если вдруг заглавная
           char from_string_j = (char)tolower(v[j][0]); // понижаю первую букву, если вдруг заглавная
           if (from_string_i > from_string_j) swap(v[i],v[j]);  // сортирую
        }
    }
}

void output_vector (const vector<string> &v)
{
    for (string s : v) cout << s << endl; // вывожу все строки массива
}

void display_menu ()
{
    cout << "*** Меню ***" << endl;
    cout << "1) Дана строка символов. Определить, сколько символов предшествует первому вхождению некоторого заданного символа (в случае отсутствия такового вывести соответствующее сообщение)." << endl;
    cout << "2) Упорядочить данный массив английских слов по алфавиту, рассматривая только первый символ каждого слова." << endl;
    cout << "3) Результаты вступительных экзаменов представлены в виде списка из N строк, в каждой строке которого записаны фамилия студента и отметки по каждому из М экзаменов. Определить количество абитуриентов, сдавших вступительные экзамены на «отлично»." << endl;
    cout << "4) Завершить работу программы." << endl;
}


int main()
{
    srand(time(0));
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №8 Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
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
                {
                    string S;
                    char c;
                    cout << "Введите строку символов -> ";
                    cin.get();
                    getline(cin, S);
                    cout << "Введенная строка -> " << S << endl;
                    cout << "Введите символ: ";
                    cin >> c;
                    cout << "Введенный символ: " << c << endl;;
                    if (ex1(S,c) != -1) cout << ex1(S,c) << " символа/ов предшествуют первому вхождению символа " << c;
                    else cout << "В строке такой символ отсутствует.";
                    break;
                }
                case 2:
                {
                    vector<string> v = zapolnenie_vectora();
                    sort_vector(v);
                    output_vector(v);
                    break;
                }
                case 3:
                {
                    int N, M;
                    cout << "Задайте количество строк N: ";
                    cin >> N;
                    while (N < 1)
                    {
                        cout << "error!" << endl;
                        cout << "Задайте количество строк N: ";
                        cin >> N;
                    }
                    cout << "Задайте количество столбцов M: ";
                    cin >> M;
                    while (M < 1)
                    {
                        cout << "error!" << endl;
                        cout << "Задайте количество столбцов N: ";
                        cin >> M;
                    }
                    vector<string> v = zapolnenie_vectora(N, M);
                    output_vector(v);
                    cout << ex3(v, N, M);
                    break;
                }
            }
            cout << endl << "Желаете вернуться в меню?" << endl;
            system("pause");
            system("cls");
        }
    }
    cout << "Программа была завершена по желанию пользователя.";
    return 0;
}
