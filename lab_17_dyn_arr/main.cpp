#include <iostream>
#include <windows.h>
#include <ctime>
#include <fstream>

using namespace std;

void SetColor(int textcolor, int backgroundcolor)
{
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)((backgroundcolor << 4) | textcolor));
}

void push (int *arr, int &top, const int &info);

//*** ОЧЕРЕДЬ *** //
void CreateQueue (int *arr, int &head, int &tail, const int &info)
{
    arr[0] = info;
    head = 0;
    tail = 0;
}

void pop (int *arr, int &head, int &tail, int &info)
{
    if (head != -1 || head != tail+1)
    {
        info = arr[head];
        head++;
    }
}

int peek(int *arr, const int &head) // просмотр info элемента
{
    return arr[head];
}

void DisplayQueue (int *arr, int &head, int &tail) // вывести стек
{
    cout << "Очередь: " << endl;
    if (head == -1 || head == tail+1) cout << "Очередь пуста." << endl;
    else
    {
        int i = head;
        while (i != tail+1)
        {
            if (i == head) SetColor(2,0);
            else if (i == tail) SetColor(3,0);
            cout << "\t" << arr[i] << " ";
            i++;
            SetColor(7,0);
        }
    }
    cout << endl;
}

void filling_queue_from_keyboard (int *arr, int &head, int &tail, int N)
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

void filling_queue_random (int *arr, int &head, int &tail, int N, int min, int max)
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

void filling_queue_from_file (int *arr, int &head, int &tail, int N, string filename)
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

void sposob_zapolneniya (int *arr, int &Head, int &Tail, int N)
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

double sred_arif (int *arr, int &head, int &tail, int &N)
{
    double summa = 0;
    int i = 0;
    if (head != -1 || head != tail+1)
    {
        i = head;
        while (i != tail+1)
        {
            summa += arr[i];
            i++;
        }
    }
    N = i;
    return (summa / i);
}

void ex2 (int *arr, int &head, int &tail)
{
    if (head != -1 || head != tail+1)
    {
        int N;
        double sredarif = sred_arif(arr, head, tail, N);

        cout << "Среднее арифметическое: " << sredarif << endl;
        for (int i = 0; i < N; i++)
        {
            int info = 1;
            pop(arr, head, tail, info);
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

void push (int *arr, int &top, const int &info)
{
    top++;
    arr[top] = info;
}

void pop (int *arr, int &top, int &info) // удаление элемента
{
    if (top != -1)
    {
        info = arr[top];
        top--;
    }
}


void filling_stack_from_keyboard (int *arr, int &top, int N)
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

void filling_stack_random (int *arr, int &top, int N, int min, int max)
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

void filling_stack_from_file (int *arr, int &top, int N, string filename)
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


void sposob_zapolneniya (int *arr,int &top, int N)
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

void DisplayStack (int *arr, const int &top) // вывести стек
{
    cout << "Стек: " << endl;
    if (top == -1) cout << "Стек пуст." << endl;
    else
    {
        for (int i = top; i != -1; i--)
        {
            if (i == top) SetColor(2,0);
            else SetColor(7,0);
            cout << "\t" << arr[i] << endl;
        }
    }
}

void CopyStacks (int *arr, int &top, int *temp_arr, int &top_temp, int &positive_count) // скопировать элементы первого стека во второй
{
    int info = 0;
    while (top != -1)
    {
        pop(arr, top,info);
        push(temp_arr,top_temp,info);
        if (info > 0) positive_count++;
    }
}

void ex1 (int *arr, int &top, int *temp_arr, int &top_temp)
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
                int *arr = new int[N]; // объявляю дин. массив
                int top = -1; // стек пуст
                sposob_zapolneniya(arr, top, N); // заполняю стек
                DisplayStack(arr, top); // вывожу стек

                int *temp_arr = new int [2*N]; // 2*N так как все числа могут быть положительными
                int top_temp = -1; // изначально он пуст

                int positive_count = 0; // количество положительных чисел
                CopyStacks(arr, top, temp_arr, top_temp, positive_count); // копирую из первого во второй
                delete[] arr; // удаляю первый

                int new_N = (N - positive_count) + (positive_count*2); // вычисляю новый размер

                arr = new int[new_N]; // выделяю для первого память

                ex1(arr, top, temp_arr, top_temp); // заполняю первый в соответствии с заданием
                delete[] temp_arr; // удаляю темповый массив

                DisplayStack(arr, top); // вывожу результат
                break;
            }
            case 2:
            {
                int *arr = new int[2*N]; // объявляю дин. массив с удвоенным размером, если вдруг удалят все элементы
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
