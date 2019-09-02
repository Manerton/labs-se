#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <vector>
#include <conio.h>
#include <locale>

using namespace std;

void SetColor(int color)
{
   HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(hStdOut, (WORD)((0 << 4) | color));
}

void creation_text (vector<wstring> &v)
{
    wifstream input("input.dat");
    if(input)
    {
        int length = 0;
        wstring temp;
        while (getline(input, temp))
        {
            if (temp.length() < 80 && length < 1000)
            {
                v.push_back(temp);
                length += temp.length();
            }
        }
    } else wcout <<L"Файл не обнаружен!" << endl;
}

void display (const vector<wstring> &v)
{
    for (wstring s : v) wcout << s << endl; // вывожу все строки массива
}

vector<wstring> wstring_to_vector_slov (const wstring &S) // преобразую предложение (строку) в вектор слов
{
    vector<wstring> vector_slov;
    int N = S.length();
    wstring slovo;
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

void find_index_word (const vector<wstring> &v, const wstring &word)
{
    wstring temp;
    int N = v.size();
    for (int i = 0; i < N; i++)
    {
        temp = v[i];
        if (temp.find(word) != wstring::npos) // Если такое слово было найдено
        {
            wcout << L"Предложение " << i+1 << ": ";
            vector<wstring> vector_slov = wstring_to_vector_slov(temp);
            int M = vector_slov.size();
            for (int j = 0; j < M; j++)
            {
                if (vector_slov[j] == word) wcout << j+1 << " ";
            }
            wcout << endl;
        }
    }
}

void highlighted_display (const vector<wstring> &v, const wstring &word)
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
                if (v[i].find(word) != wstring::npos) // Если такое слово было найдено
                {
                    vector<wstring> vector_slov = wstring_to_vector_slov(v[i]);
                    int M = vector_slov.size();
                    SetColor(2);
                    for (int j = 0; j < M; j++)
                    {
                        if (vector_slov[j] == word)
                        {
                            SetColor(3);
                            wcout << vector_slov[j] << " ";
                            SetColor(2);
                        } else wcout << vector_slov[j] << " ";
                    }
                    SetColor(7);
                    wcout << endl;
                }
                else
                {
                    SetColor(2);
                    wcout << v[i] << endl;
                    SetColor(7);
                }
            } else wcout << v[i] << endl;
        }
    }
}
int main()
{
    setlocale( LC_ALL, "rus" );
    system("chcp 1251 > nul");
    wcout << L"Лабораторная работа №12 Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
    vector<wstring> v;
    creation_text(v);
    wcout << L"Вывод текста на экран: " << endl;
    display(v);
    wstring word;
    wcout << L"Определить порядковый номер заданного слова в каждом предложении текста (заданное слово вводится с клавиатуры): " << endl;
    wcout << L"Заданное слово: ";
    wcin >> word;
    find_index_word(v, word);
    system("pause");
    highlighted_display(v,word);
    return 0;
}
