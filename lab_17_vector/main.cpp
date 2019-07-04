#include <iostream>
#include <vector>
#include <windows.h>
#include <fstream>
#include <ctime>

using namespace std;

void SetColor(int textcolor, int backgroundcolor)
{
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)((backgroundcolor << 4) | textcolor));
}

//*** ОЧЕРЕДЬ *** //
void CreateQueue (vector<int> &arr, int &head, int &tail, const int &info)
{
    arr.push_back(info);
    head = 0;
    tail = 0;
}

void pop (vector<int> &arr, int &head, int &tail, int &info)
{
    if (tail != -1)
    {
        info = arr.at(head);
        arr.erase(arr.begin());
        tail--;
    }
}

void push (vector<int> &arr, int &tail, const int &info)
{
    tail++;
    arr.push_back(info);
}

int peek(const vector<int> &arr, const int &top) // просмотр info элемента
{
    return arr.at(top);
}

void DisplayQueue (vector<int> &arr, int &head, int &tail) // вывести стек
{
    cout << "Очередь: " << endl;
    if (tail == -1) cout << "Очередь пуста." << endl;
    else
    {
        int i = head;
        while (i != tail+1)
        {
            if (i == head) SetColor(2,0);
            else if (i == tail) SetColor(3,0);
            cout << "\t" << arr.at(i) << " ";
            i++;
            SetColor(7,0);
        }
    }
    cout << endl;
}

void filling_queue_from_keyboard (vector<int> &arr, int &head, int &tail, int N)
{
    cout << "*** Осуществляется заполнение очереди значениями с клавиатуры ***" << endl;
    cout << "Задайте значения элементов очереди: " << endl;
    int chislo;

    cout << "Для " << 1 << "-го: ";
    cin >> chislo;
    CreateQueue(arr, head, tail, chislo);

    for (int i = 1; i < N; i++)
    {
        cout << "Для " << i+1 << "-го: ";
        cin >> chislo;
        push(arr, tail, chislo);
    }
}

void filling_queue_random (vector<int> &arr, int &head, int &tail, int N, int min, int max)
{
    cout << "*** Осуществляется заполнение очереди случайными целыми числами ***" << endl;
    int chislo;
    int ostatok = (max - min) + 1;

    chislo = min + rand() % ( ostatok );
    CreateQueue(arr, head, tail, chislo);

    for (int i = 1; i < N; i++)
    {
        chislo = min + rand() % ( ostatok );
        push(arr, tail, chislo);
    }
}

void filling_queue_from_file (vector<int> &arr, int &head, int &tail, int N, string filename)
{
    cout << "*** Осуществляется заполнение стека из файла " << filename << " ***" << endl;
    int chislo;
    ifstream input(filename);
    if (input) // проверка открытия
    {
        input >> chislo;
        CreateQueue(arr, head, tail, chislo);
        for (int i = 1; i < N; i++)
        {
            input >> chislo;
            push(arr, tail, chislo);
        }
        input.close();
    } else cout << endl << "error! Не удалось открыть файл " << filename << endl;
}

void diapazon_rand (int &min, int& max)
{
    cout << "Задайте диапазон генерирования случайных чисел." << endl;
    cout << "Нижняя граница: ";
    cin >> min;
    cout << "Верхняя граница: ";
    cin >> max;
    while (min >= max)
    {
        cout << "error!" << endl;
        cout << "Верхняя граница: ";
        cin >> max;
    }
}

void sposob_zapolneniya (vector<int> &arr, int &Head, int &Tail, int N)
{
    int choice;
    cout << "*** Выбор способа заполнения ***" << endl;
    cout << "1. Заполнение с клавиатуры." << endl;
    cout << "2. Заполнение случайными числами." << endl;
    cout << "3. Заполнение из файла." << endl;
    cout << "Выберите способ заполнения: ";
    cin >> choice;
    while (choice < 1 || choice > 3)
    {
        cout << "error!" << endl;
        cout << "Выберите способ заполнения: ";
        cin >> choice;
    }
    switch (choice)
    {
    case 1:
    {
        filling_queue_from_keyboard(arr,Head,Tail, N);
        break;
    }
    case 2:
    {
        int min, max;
        diapazon_rand(min,max);
        filling_queue_random(arr,Head,Tail,N,min,max);
        break;
    }
    case 3:
        string filename;
        cout << "Введите название файла: ";
        cin >> filename;
        filling_queue_from_file(arr,Head,Tail,N,filename);
        break;
    }
}

double sred_arif (vector<int> &arr, int &head, int &tail, int &N)
{
    double summa = 0;
    int i = 0;
    if (tail != -1)
    {
        i = head;
        while (i != tail+1)
        {
            summa += arr.at(i);
            i++;
        }
    }
    N = i;
    return (summa / i);
}

void ex2 (vector<int> &arr, int &head, int &tail)
{
    if (tail != -1)
    {
        int N;
        double sredarif = sred_arif(arr, head, tail, N);

        cout << "Среднее арифметическое: " << sredarif << endl;
        for (int i = 0; i < N; i++)
        {
            int info = 1;
            pop(arr, head,tail, info);
            double difference = sredarif / info;
            if (difference < 1 && difference > 0) difference = 1 / difference;
            if (difference <= 1.7 && difference >= 1)
            {
                push(arr, tail, info);
            }
        }
    }
}

// *** КОНЕЦ ОЧЕРЕДИ *** //

void pop (vector<int> &arr, int &top, int &info) // удаление элемента
{
    if (top != -1)
    {
        info = arr.at(top);
        arr.pop_back();
        top--;
    }
}

void filling_stack_from_keyboard (vector<int> &arr, int &top, int N)
{
    cout << "*** Осуществляется заполнение стека значениями с клавиатуры ***" << endl;
    cout << "Задайте значения элементов стека: " << endl;
    int chislo;
    for (int i = 0; i < N; i++)
    {
        cout << "Для " << i+1 << "-го: ";
        cin >> chislo;
        push(arr, top, chislo);
    }
}

void filling_stack_random (vector<int> &arr, int &top, int N, int min, int max)
{
    cout << "*** Осуществляется заполнение стека случайными целыми числами ***" << endl;
    int chislo;
    int ostatok = (max - min) + 1;
    for (int i = 0; i < N; i++)
    {
        chislo = min + rand() % ( ostatok );
        push(arr, top, chislo);
    }
}

void filling_stack_from_file (vector<int> &arr, int &top, int N, string filename)
{
    cout << "*** Осуществляется заполнение стека из файла " << filename << " ***" << endl;
    int chislo;
    ifstream input(filename);
    if (input) // проверка открытия
    {
        for (int i = 0; i < N; i++)
        {
            input >> chislo;
            push(arr, top, chislo);
        }
        input.close();
    } else cout << endl << "error! Не удалось открыть файл " << filename << endl;
}

void sposob_zapolneniya (vector<int> &arr,int &top, int N)
{
    int choice;
    cout << "*** Выбор способа заполнения ***" << endl;
    cout << "1. Заполнение с клавиатуры." << endl;
    cout << "2. Заполнение случайными числами." << endl;
    cout << "3. Заполнение из файла." << endl;
    cout << "Выберите способ заполнения: ";
    cin >> choice;
    while (choice < 1 || choice > 3)
    {
        cout << "error!" << endl;
        cout << "Выберите способ заполнения: ";
        cin >> choice;
    }
    switch (choice)
    {
    case 1:
    {
        filling_stack_from_keyboard(arr, top, N);
        break;
    }
    case 2:
    {
        int min, max;
        diapazon_rand(min,max);
        filling_stack_random(arr, top, N,min,max);
        break;
    }
    case 3:
        string filename;
        cout << "Введите название файла: ";
        cin >> filename;
        filling_stack_from_file(arr,top,N,filename);
        break;
    }
}

void DisplayStack (const vector<int> &arr, const int &top) // вывести стек
{
    cout << "Стек: " << endl;
    if (top == -1) cout << "Стек пуст." << endl;
    else
    {
        for (int i = top; i != -1; i--)
        {
            if (i == top) SetColor(2,0);
            else SetColor(7,0);
            cout << "\t" << arr.at(i) << endl;
        }
    }
}

void CopyStacks (vector<int> &arr, int &top, vector<int> &temp_arr, int &top_temp) // скопировать элементы первого стека во второй
{
    int info = 0;
    while (top != -1)
    {
        pop(arr, top,info);
        push(temp_arr,top_temp,info);
    }
}

void ex1 (vector<int> &arr, int &top, vector<int> &temp_arr, int &top_temp)
{
    while (top_temp != -1)
    {
        int info;
        pop(temp_arr, top_temp, info);
        if (info > 0)
        {
            push(arr,top, 0);
        }
        push(arr,top,info);
    }
}


int main()
{
    system("chcp 1251 > nul");
    srand(time(0));
    cout << "Лабораторная работа №17. Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;

    int ch;
    cout << "1. Создать стек из чисел. После каждого положительного числа вставить нуль." << endl;
    cout << "2. Создать очередь из чисел. Удалить из очереди все элементы, отличающиеся от среднего арифметического более, чем на 70%." << endl;
    cout << "0. Завершить работу программы." << endl;
    cout << "Ввод: ";
    cin >> ch;
    while (ch < 0 || ch > 2)
    {
        cout << "Ошибка!" << endl;
        cout << "Выберите пункт меню: ";
        cin >> ch;
    }
    if (ch != 0)
    {
        int N;
        cout << "Задайте размер N: ";
        cin >> N;
        // Проверка на дурака
        while (N < 1)
        {
            cout << "error!" << endl;
            cout << "Задайте размер N: ";
            cin >> N;
        }
        // Конец ввода
        switch (ch)
        {
        case 1:
            {
                vector<int> arr;
                int top = -1; // стек пуст
                sposob_zapolneniya(arr, top, N);
                DisplayStack(arr, top);

                vector<int> temp_arr;
                int top_temp = -1;

                CopyStacks(arr, top, temp_arr, top_temp);

                ex1(arr, top, temp_arr, top_temp);

                DisplayStack(arr, top);
                break;
            }
            case 2:
            {
                vector<int> arr;
                int Head = -1; // очередь пуста
                int Tail = -1; // очередь пуста
                sposob_zapolneniya(arr, Head, Tail, N);
                DisplayQueue(arr, Head, Tail);
                ex2(arr, Head,Tail);
                DisplayQueue(arr, Head, Tail);
                break;
            }
        }
    }

    return 0;
}
