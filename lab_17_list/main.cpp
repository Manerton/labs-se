#include <iostream>
#include <windows.h>
#include <ctime>
#include <fstream>

using namespace std;

struct Stack
{
    int info;
    Stack *next;
};

struct Queue
{
    int info;
    Queue *next;
};

typedef Stack *PStack;
typedef Queue *PQueue;

PStack t; // текущий элемент стека
PQueue q; // текущий элемент очереди


void SetColor(int textcolor, int backgroundcolor)
{
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)((backgroundcolor << 4) | textcolor));
}
//*** ОЧЕРЕДЬ *** //
void CreateQueue (PQueue &Head, PQueue &Tail, const int &info)
{
    q = new Queue;
    q -> info = info;
    q -> next = nullptr;
    Head = q;
    Tail = q;
}

void push (PQueue &Tail, const int &info)
{
    q = new Queue;
    q -> info = info;
    q -> next = nullptr;
    Tail -> next = q;
    Tail = q;
}

void pop (PQueue &Head, int &info)
{
    if (Head != nullptr)
    {
        q = Head;
        info = q->info;
        Head = Head->next;
        delete q;
    }
}

int peek(PQueue &Head) // просмотр info элемента
{
    return Head -> info;
}

void DisplayQueue (PQueue &Head, PQueue &Tail) // вывести стек
{
    cout << "Очередь: " << endl;
    if (Head == nullptr) cout << "Очередь пуста." << endl;
    else
    {
        q = Head;
        while (q != nullptr)
        {
            if (q == Head) SetColor(2,0);
            else if (q == Tail) SetColor(3,0);
            cout << "\t" << q -> info << " ";
            q = q -> next;
            SetColor(7,0);
        }
    }
    cout << endl;
}

void filling_queue_from_keyboard (PQueue &Head, PQueue &Tail, int N)
{
    cout << "*** Осуществляется заполнение очереди значениями с клавиатуры ***" << endl;
    cout << "Задайте значения элементов очереди: " << endl;
    int chislo;

    cout << "Для " << 1 << "-го: ";
    cin >> chislo;
    CreateQueue(Head, Tail, chislo);

    for (int i = 1; i < N; i++)
    {
        cout << "Для " << i+1 << "-го: ";
        cin >> chislo;
        push(Tail, chislo);
    }
}

void filling_queue_random (PQueue &Head, PQueue &Tail, int N, int min, int max)
{
    cout << "*** Осуществляется заполнение очереди случайными целыми числами ***" << endl;
    int chislo;
    int ostatok = (max - min) + 1;

    chislo = min + rand() % ( ostatok );
    CreateQueue(Head, Tail, chislo);

    for (int i = 1; i < N; i++)
    {
        chislo = min + rand() % ( ostatok );
        push(Tail, chislo);
    }
}

void filling_queue_from_file (PQueue &Head, PQueue &Tail, int N, string filename)
{
    cout << "*** Осуществляется заполнение очереди из файла " << filename << " ***" << endl;
    int chislo;
    ifstream input(filename);
    if (input) // проверка открытия
    {
        input >> chislo;
        CreateQueue(Head, Tail, chislo);
        for (int i = 1; i < N; i++)
        {
            input >> chislo;
            push(Tail, chislo);
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

void sposob_zapolneniya (PQueue &Head, PQueue &Tail, int N)
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
        filling_queue_from_keyboard(Head,Tail, N);
        break;
    }
    case 2:
    {
        int min, max;
        diapazon_rand(min,max);
        filling_queue_random(Head, Tail,N,min,max);
        break;
    }
    case 3:
        string filename;
        cout << "Введите название файла: ";
        cin >> filename;
        filling_queue_from_file(Head,Tail,N,filename);
        break;
    }
}

double sred_arif (PQueue &Head, int &N)
{
    double summa = 0;
    int i = 0;
    if (Head != nullptr)
    {
        q = Head;
        while (q != nullptr)
        {
            summa += q->info;
            q = q -> next;
            i++;
        }
    }
    N = i;
    return (summa / i);
}

void ex2 (PQueue &Head, PQueue &Tail)
{
    if (Head != nullptr)
    {
        int N;
        double sredarif = sred_arif(Head, N);
       // double lowborder = sredarif / 1.7;
       // double highborder = sredarif * 1.7;

        cout << "Среднее арифметическое: " << sredarif << endl;
        for (int i = 0; i < N; i++)
        {
            int info;
            pop(Head, info);
           // if (info >= lowborder && info <= highborder)
            double difference = sredarif / info;
            if (difference < 1 && difference > 0) difference = 1 / difference;
            if (difference <= 1.7 && difference >= 1)
            {
                push(Tail, info);
            }
        }
    }
}

// *** КОНЕЦ ОЧЕРЕДИ *** //

// *** СТЕК *** //

void push (PStack &Head, const int &info) // если создаем впервые, то создаем стек, иначе добавляем в стек новый элемент
{
    t = new Stack; // выделяем память для нового элемента
    t -> info = info; // формируем информационную часть
    t -> next = Head; // в адресную часть пишем значение вершины
    Head = t; // в вершину пишем значение адреса нового элемента
}

void pop (PStack &Head, int &info) // удаление элемента
{
    if (Head != nullptr) // если стек пустой, то ничего не делаем
    {
        t = Head;
        info = t -> info;
        Head = t->next;
        delete t;
    }
}

int peek(PStack &Head) // просмотр info элемента
{
    return Head -> info;
}

void filling_stack_from_keyboard (PStack &Head, int N)
{
    cout << "*** Осуществляется заполнение стека значениями с клавиатуры ***" << endl;
    cout << "Задайте значения элементов стека: " << endl;
    int chislo;
    for (int i = 0; i < N; i++)
    {
        cout << "Для " << i+1 << "-го: ";
        cin >> chislo;
        push(Head, chislo);
    }
}

void filling_stack_random (PStack &Head, int N, int min, int max)
{
    cout << "*** Осуществляется заполнение стека случайными целыми числами ***" << endl;
    int chislo;
    int ostatok = (max - min) + 1;
    for (int i = 0; i < N; i++)
    {
        chislo = min + rand() % ( ostatok );
        push(Head, chislo);
    }
}

void filling_stack_from_file (PStack &Head, int N, string filename)
{
    cout << "*** Осуществляется заполнение стека из файла " << filename << " ***" << endl;
    int chislo;
    ifstream input(filename);
    if (input) // проверка открытия
    {
        for (int i = 0; i < N; i++)
        {
            input >> chislo;
            push(Head, chislo);
        }
        input.close();
    } else cout << endl << "error! Не удалось открыть файл " << filename << endl;
}

void sposob_zapolneniya (PStack &Head,int N)
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
        filling_stack_from_keyboard(Head, N);
        break;
    }
    case 2:
    {
        int min, max;
        diapazon_rand(min,max);
        filling_stack_random(Head, N,min,max);
        break;
    }
    case 3:
        string filename;
        cout << "Введите название файла: ";
        cin >> filename;
        filling_stack_from_file(Head,N,filename);
        break;
    }
}

void DisplayStack (PStack &Head) // вывести стек
{
    cout << "Стек: " << endl;
    if (Head == nullptr) cout << "Стек пуст." << endl;
    else
    {
        t = Head;
        while (t != nullptr)
        {
            if (t == Head) SetColor(2,0);
            else SetColor(7,0);
            cout << "\t" << t -> info << endl;
            t = t -> next;
        }
    }
}

void CopyStacks (PStack &FromHead, PStack &ToHead) // скопировать элементы первого стека во второй (очистив при этом первый стек)
{
    int info = 0;
    while (FromHead != nullptr)
    {
        pop(FromHead,info);
        push(ToHead,info);
    }
}

void ex1 (PStack &Head)
{
    PStack HeadTemp = nullptr;
    CopyStacks(Head, HeadTemp);
    while (HeadTemp != nullptr)
    {
        int info;
        pop(HeadTemp, info);
        if (info > 0)
        {
            push(Head, 0);
        }
        push(Head, info);
    }
}
// ** КОНЕЦ СТЕКА ** //

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
                PStack Head = nullptr;
                sposob_zapolneniya(Head, N);
                DisplayStack(Head);
                ex1(Head);
                DisplayStack(Head);
                break;
            }
            case 2:
            {
                PQueue Head;
                PQueue Tail;
                sposob_zapolneniya(Head, Tail, N);
                DisplayQueue(Head, Tail);
                ex2(Head,Tail);
                DisplayQueue(Head, Tail);
                break;
            }
        }
    }
    return 0;
}
