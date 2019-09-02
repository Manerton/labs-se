#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>
#include <vector>
#include <conio.h>

using namespace std;

const int len = 80;
const int size = 13;

void cin_clean_buffer ()
{
    cin.clear();
    while (cin.get() != '\n');
}

void SetColor(int color)
{
   HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(hStdOut, (WORD)((0 << 4) | color));
}

void creation_text (char M[size][len], int &N)
{
    ifstream input("input.dat");
    if(input)
    {
        int length = 0;
        char S[len];
        int i = 0;
        while (input.getline(S, 80))
        {
            if (length < 1000)
            {
                strcpy(M[i], S);
                length += strlen(S);
                i++;
                N++;
            }
        }
    } else cout <<"Файл не обнаружен!" << endl;
}

void display (char M[size][len], int N)
{
    for (int i = 0; i < N; i++)
    {
        cout << M[i] << endl;
    }
}

void find_index_word (char M[size][len], int N, char word[len])
{
    char S[len];
    for (int i = 0; i < N; i++)
    {
        strcpy(S,M[i]);
        if (strstr(S,word) != NULL) // Если такое слово было найдено
        {
            cout << "Предложение " << i+1 << ": ";
            char *pch = strtok(S," ");
            int j = 0;
            while (pch != NULL)
            {
                if (strcmp(pch,word) == 0) cout << j+1 << " ";
                pch = strtok(NULL," "); // На месте первого параметра - нулевой указатель, в этом случае функция продолжает сканирование с того места, где был остановлен предыдущий успешный вызов функции.
                j++;
            }
            cout << endl;
        }
    }
}

void highlighted_display (char M[size][len], int N, char word[len])
{
    system("cls");
    display(M, N);
    for (int count = 0; count < N; count++)
    {
        getch();
        system("cls");

        char S[len];

        for (int i = 0; i < N; i++)
        {
            if (i == count)
            {
                SetColor(2);
                strcpy(S,M[i]);
                if (strstr(S,word) != NULL) // Если такое слово было найдено
                {
                    char *pch = strtok(S," ");
                    while (pch != NULL)
                    {
                        if (strcmp(pch,word) == 0)
                        {
                            SetColor(3);
                            cout << pch << " ";
                            SetColor(2);
                        } else cout << pch << " ";
                        pch = strtok(NULL," ");
                    }
                    SetColor(7);
                    cout << endl;
                }
                else
                {
                    cout << M[i] << endl;
                    SetColor(7);
                }
            } else cout << M[i] << endl;
        }
    }
}
int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №12 Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
    char M [size][len];
    int N = 0;
    creation_text(M, N);
    cout << "Вывод текста на экран: " << endl;
    display(M, N);
    char word[len];
    cout << "Определить порядковый номер заданного слова в каждом предложении текста (заданное слово вводится с клавиатуры): " << endl;
    cout << "Заданное слово: ";
    cin.getline(word,80);
    if (cin.fail()) cin_clean_buffer(); // обрабатываю случай, когда ввод более 60 символов
    find_index_word(M, N, word);
    system("pause");
    highlighted_display(M, N, word);
    return 0;
}
