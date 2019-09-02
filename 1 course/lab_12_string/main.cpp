#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <vector>
#include <conio.h>

using namespace std;

void SetColor(int color)
{
   HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(hStdOut, (WORD)((0 << 4) | color));
}

void creation_text (vector<string> &v)
{
    ifstream input("input.dat");
    if(input)
    {
        int length = 0;
        string temp;
        while (getline(input, temp))
        {
            if (temp.length() < 80 && length < 1000)
            {
                v.push_back(temp);
                length += temp.length();
            }
        }
    } else cout <<"Файл не обнаружен!" << endl;
}

void display (const vector<string> &v)
{
    for (string s : v) cout << s << endl; // вывожу все строки массива
}

vector<string> string_to_vector_slov (const string &S) // преобразую предложение (строку) в вектор слов
{
    vector<string> vector_slov;
    int N = S.length();
    string slovo;
    for (int i = 0; i < N; i++)
    {
        if (S[i] != ' ' && i != N-1) // пока не нашел пробел или не дошел до конца строки
        {
            slovo += S[i];
        }
        else if (i == N-1) // если дошел до конца строки
        {
            slovo += S[i];
            vector_slov.push_back(slovo);
        }
        else // если нашел пробел
        {
            vector_slov.push_back(slovo);
            slovo.clear();
        }
    }
    return vector_slov;
}

void find_index_word (const vector<string> &v, const string &word)
{
    string temp;
    int N = v.size();
    for (int i = 0; i < N; i++)
    {
        temp = v[i];
        if (temp.find(word) != string::npos) // Если такое слово было найдено
        {
            cout << "Предложение " << i+1 << ": ";
            vector<string> vector_slov = string_to_vector_slov(temp);
            int M = vector_slov.size();
            for (int j = 0; j < M; j++)
            {
                if (vector_slov[j] == word) cout << j+1 << " ";
            }
            cout << endl;
        }
    }
}

void highlighted_display (const vector<string> &v, const string &word)
{
    int N = v.size();
    system("cls");
    display(v);
    for (int count = 0; count < N; count++)
    {
        getch();
        system("cls");
        for (int i = 0; i < N; i++)
        {
            if (i == count)
            {
                if (v[i].find(word) != string::npos) // Если такое слово было найдено
                {
                    vector<string> vector_slov = string_to_vector_slov(v[i]);
                    int M = vector_slov.size();
                    SetColor(2);
                    for (int j = 0; j < M; j++)
                    {
                        if (vector_slov[j] == word)
                        {
                            SetColor(3);
                            cout << vector_slov[j] << " ";
                            SetColor(2);
                        } else cout << vector_slov[j] << " ";
                    }
                    SetColor(7);
                    cout << endl;
                }
                else
                {
                    SetColor(2);
                    cout << v[i] << endl;
                    SetColor(7);
                }
            } else cout << v[i] << endl;
        }
    }
}
int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №12 Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
    vector<string> v;
    creation_text(v);
    cout << "Вывод текста на экран: " << endl;
    display(v);
    string word;
    cout << "Определить порядковый номер заданного слова в каждом предложении текста (заданное слово вводится с клавиатуры): " << endl;
    cout << "Заданное слово: ";
    cin >> word;
    find_index_word(v, word);
    system("pause");
    highlighted_display(v,word);
    return 0;
}
