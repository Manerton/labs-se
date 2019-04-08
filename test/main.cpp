// ver 1.1 - 08.04.19 - Вроде работает
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

using namespace std;

struct Node
{
    short int elem;
    Node *next;
};
typedef Node *PNode;
PNode Head;

 // объявляю начало списка

//создает новый узел в памяти и возвращает его адрес
PNode CreateNode (short int NewElem)
{
    PNode NewNode = new Node;
    NewNode->elem = NewElem;
    NewNode->next = NULL;
    return NewNode;
}
// добавить в начало списка
void AddFirst(PNode &Head, PNode NewNode)
{
    NewNode->next = Head; // делаем, чтобы у NewNode ссылка вела на предыдущую голову списка
    Head = NewNode; // делаем новой головой NewNode
}
void AddAfter (PNode p, PNode NewNode)
{
    NewNode->next = p->next;
    p->next = NewNode;
}
// добавить в конец списка
void AddLast(PNode &Head, PNode NewNode)
{
    PNode q = Head;
    if (Head == NULL) // список пуст
    {
        AddFirst(Head, NewNode);
        return;
    }
    while (q->next) q = q->next;
    AddAfter(q, NewNode);
}

// проверка на пустоту
bool empty_list (PNode &Head)
{
    if (Head == NULL) return true;
    else return false;
}
// вывод листа
void print_list (PNode &Head)
{
    PNode q = Head;
    if (empty_list(Head)) cout << "Лист пуст." << endl;
    else
    {
        while (q)
        {
            cout << q->elem << " ";
            q = q->next;
        }
        cout << endl;
    }
}
void filling_list_from_keyboard (PNode &Head, int N)
{
    cout << "*** Осуществляется заполнение односвязного списка значениями с клавиатуры ***" << endl;
    cout << "Задайте значения элементов односвязного списка: " << endl;
    int chislo;
    PNode NewNode;
    for (int i = 0; i < N; i++)
    {
        cout << "Для " << i+1 << "-го: ";
        cin >> chislo;
        NewNode = CreateNode(chislo);
        AddLast(Head, NewNode);
    }
}

void filling_list_random (PNode &Head, int N, int min, int max)
{
    cout << "*** Осуществляется заполнение односвязного списка случайными целыми числами ***" << endl;
    int chislo;
    PNode NewNode;
    int ostatok = (max - min) + 1;
    for (int i = 0; i < N; i++)
    {
        chislo = min + rand() % ( ostatok );
        NewNode = CreateNode(chislo);
        AddLast(Head, NewNode);
    }
}

void filling_list_from_file (PNode &Head, int N, string filename)
{
    cout << "*** Осуществляется заполнение односвязного списка из файла " << filename << " ***" << endl;
    int chislo;
    PNode NewNode;
    ifstream input(filename);
    if (input) // проверка открытия
    {
        for (int i = 0; i < N; i++)
        {
            input >> chislo;
            NewNode = CreateNode(chislo);
            AddLast(Head, NewNode);
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

void sposob_zapolneniya (PNode &Head, int N)
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
        filling_list_from_keyboard(Head, N);
        break;
    }
    case 2:
    {
        int min, max;
        diapazon_rand(min,max);
        filling_list_random(Head, N,min,max);
        break;
    }
    case 3:
        string filename;
        cout << "Введите название файла: ";
        cin >> filename;
        filling_list_from_file(Head, N,filename);
        break;
    }
}

//Проверка на полный квадрат (если корень double числа равен корню приведенному к int, то число - полный квадрат)
bool full_square (short int chislo)
{
    double sqrt_chislo = sqrt(chislo);
    if (chislo >= 0 && sqrt_chislo == static_cast<short int> (sqrt_chislo)) return true;
    else return false;
}

void display_menu ()
{
    cout << "*** Меню ***" << endl;
    cout << "1. Задание 1.1 - Добавить первый элемент, являющийся полным квадратом после числа, в которых нет заданной цифры." << endl;
    cout << "2. Задание 1.2 - Добавить первый элемент, являющийся полным квадратом перед числом, в котором нет заданной цифры." << endl;
    cout << "3. Задание 2 - Удалить все числа Фибоначчи." << endl;
    cout << "4. Завершить работу программы." << endl;
}
int main()
{
    srand((unsigned)time(0));
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №6 Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
    // Первый вывод условий меню
    display_menu();
    PNode Head = NULL;
    // Ввод размера списка
    int N;
    cout << endl << "Задайте размер односвязного списка: ";
    cin >> N;
    // Проверка на дурака
    while (N < 1)
    {
        cout << "error!" << endl;
        cout << "Задайте размер односвязного списка: ";
        cin >> N;
    }
    // Конец ввода

    // Список (массив, вектор) заполняется один раз
    sposob_zapolneniya(Head, N);
    cout << "Полученный односвязный список: ";
    print_list(Head);

    return 0;

}
