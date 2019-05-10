#include <iostream>
#include <cstring>
#include <vector>
#include <ctime>
#include <cstdio>

using namespace std;

const int len = 60;
const int size = 10;

void cin_clean_buffer ()
{
    cin.clear();
    while (cin.get() != '\n');
}


int ex1 (const char *S, char c)
{
    if (strchr(S,c) != nullptr) // Если такой символ был найден
    {
        char *ptr = strchr(S,c);
        return (ptr - S); // то вернуть его позицию
    } else return -1;
}

bool eng_word (const char *S) // если символы в строке все находятся в диапазоне английских букв, то true
{
    int N = strlen(S);
    for (int i = 0; i < N; i++)
    {
        if (S[i] < 'A' || S[i] > 'z') return false;
    }
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

void zapolnenie_massiva (char M[size][len], int &i) // i - кол-во строк
{
    char S[len];
    cout << "Допустимо ввести нее более 10 строк!" << endl;
    cout << "Если вы хотите прекратить ввод, введите вместо слова число 1! " << endl;
    while (S[0] != '1' && i < size)
    {
        cout << "Введите английское слово (не более 60 символов) -> ";

        cin.getline(S, len);
        if (cin.fail()) cin_clean_buffer();

        if (eng_word(S))
        {
            strcpy(M[i], S);
            i++;
        }
        else if (S[0] != '1') cout << "Слово не является английским, либо в слове присутствует лишний знак." << endl;
    }
}

void zapolnenie_massiva (char arr[size][len], int N, int M)
{
    char S[len];
    for (int i = 0; i < N; i++)
    {
        cout << "Введите фамилию студента на русском языке (с большой буквы, не более 40 символов): ";

        cin.getline(S, 40);
        if (cin.fail()) cin_clean_buffer();

        while (!secondname(S))
        {
            cout << "Слово не является фамилией, либо в слове присутствует лишний знак." << endl;
            cout << "Повторите ввод: ";

            cin.getline(S, 40);
            if (cin.fail()) cin_clean_buffer();
        }
        for (int j = 0; j < M; j++)
        {
            int random = 2 + rand() % 4; // рандом от 2 до 5
            char random_char = random + '0'; // превращаю в символ
            int str_len = strlen(S); // узнаю длину строки
            S[str_len+2] = S[str_len]; // конец строки перемещаю на 2 символа вправо
            S[str_len+1] = random_char;
            S[str_len] = ' '; // после строки вставляю пробел и рандомное число (оценка)
        }
        strcpy(arr[i],S); // копирую в массив строк строку S
    }
}

int ex3 (const char arr[size][len], int N, int M)
{
    int result = 0;
    for (int i = 0; i < N; i++)
    {
        int count = 0;
        int str_len = strlen(arr[i]);
        for (int j = 0; j < str_len; j++) // проверяю каждый символ слова, если кол-во найденных пятерок равен M, то
        {
            if (arr[i][j] == '5') count++;
        }
        if (count == M) result++; // прибавляю к результату (кол-во отличников) единицу
    }
    return result;
}

void sort_massiv (char M[size][len], const int N)
{
    for (int i = 0; i != N-1; i++)
    {
        for (int j = i+1; j != N; j++)
        {
           char from_string_i = (char)tolower(M[i][0]); // понижаю первую букву, если вдруг заглавная
           char from_string_j = (char)tolower(M[j][0]); // понижаю первую букву, если вдруг заглавная
           if (from_string_i > from_string_j) swap(M[i],M[j]);  // сортирую
        }
    }
}

void output_massiv (char M[size][len], const int N)
{
    for (int i = 0; i < N; i++)
    {
        cout << M[i] << endl;
    }
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
            cin_clean_buffer();
            switch (menu) // переходим непосредственно к выполнению заданий
            {
                case 1:
                {
                    char S[len]; // объявляю строку на 60 символов
                    char c;
                    cout << "Введите строку символов (не более 60 символов) -> ";
                    cin.getline(S,len); // ввожу линию на 60 символов
                    if (cin.fail()) cin_clean_buffer(); // обрабатываю случай, когда ввод более 60 символов
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
                    char arr[size][len]; // объявляю массив строк (массив массивов символов)
                    int count_strings(0); // кол-во строк изначально равен нулю
                    zapolnenie_massiva(arr, count_strings);
                    sort_massiv(arr, count_strings);
                    output_massiv(arr, count_strings);
                    break;
                }
                case 3:
                {
                    char arr[size][len]; // объявляю массив строк
                    int N, M;
                    cout << "Задайте количество строк N (не более 10): ";
                    cin >> N;
                    while (N < 1 || N > size)
                    {
                        cout << "error!" << endl;
                        cout << "Задайте количество строк N : ";
                        cin >> N;
                    }
                    cout << "Задайте количество столбцов M (не более 5): ";
                    cin >> M;
                    while (M < 1 || M > 5)
                    {
                        cout << "error!" << endl;
                        cout << "Задайте количество столбцов N: ";
                        cin >> M;
                    }
                    cin_clean_buffer(); // очищаю буффер перед заполнением
                    zapolnenie_massiva(arr, N, M);
                    output_massiv(arr,N);
                    cout << ex3(arr, N, M);
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
