// ver 1.0 - 10.04.19 / вроде работает
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

using namespace std;

struct Node
{
    short int elem;
    Node *next, *prev;
};
typedef Node *PNode;

PNode Head, Tail; // объявляю начало и конец списка

//создает новый узел в памяти и возвращает его адрес
PNode CreateNode (short int NewElem)
{
    PNode NewNode = new Node;
    NewNode->elem = NewElem;
    NewNode->next = NULL;
    NewNode->prev = NULL;
    return NewNode;
}
// добавить в начало списка
void AddFirst(PNode NewNode)
{
    NewNode->next = Head; // делаем, чтобы у NewNode ссылка вела на предыдущую голову списка
    NewNode->prev = NULL; // так как prev у головы должен быть null, а NewNode - новая голова
    if ( Head ) Head->prev = NewNode; // если голова в списке была, то прыдыдущий элемент для прошлой головы - это NewNode
    Head = NewNode; // делаем новой головой NewNode
    if ( ! Tail ) Tail = Head; // если при добавлении элемента список был пуст, делаем чтобы и хвост, и голова = NewNode;
}
// добавить в конец списка
void AddLast(PNode NewNode)
{
    NewNode->prev = Tail; // делаем, чтобы у NewNode предыдущий элемент был прошлый хвост
    NewNode->next = NULL; // так как next у хвоста должен быть null, а NewNode - новый хвост
    if ( Tail ) Tail->next = NewNode; // если хвост в списке был, то следующий элемент для прошлого хвоста - это NewNode
    Tail = NewNode; // делаем новым хвостом NewNode
    if ( ! Head ) Head = Tail; // если при добавлении элемента список был пуст, делаем чтобы и хвост, и голова = NewNode;
}
// поиск элемента по ключу
PNode Find (short int NewElem)
{
    PNode q = Head;
    while (q && (q->elem != NewElem))
        q = q->next;
    return q;
}
// добавить элемент после заданного
void AddAfter (PNode p, PNode NewNode)
{
    if ( ! p->next ) AddLast (NewNode); // если p - последний элемент, то добавляем NewNode в конец списка
    else
    {
        NewNode->next = p->next;
        NewNode->prev = p;
        p->next->prev = NewNode;
        p->next = NewNode;
    }
}
// добавить элемент перед заданным
void AddBefore(PNode p, PNode NewNode)
{
    if ( ! p->prev ) AddFirst (NewNode); // если p - первый элемент, то добавляем NewNode в начало списка
    else
    {
        NewNode->next = p;
        NewNode->prev = p->prev;
        p->prev->next = NewNode;
        p->prev = NewNode;
    }
}
// удалить элемент списка
void DeleteNode(PNode OldNode)
{
    if (Head == OldNode) // Если удаляем первый элемент
    {
        Head = OldNode->next;
        if ( Head ) Head->prev = NULL; // если список не опустел
        else Tail = NULL; // если список опустел
    }
    else
    {
        OldNode->prev->next = OldNode->next; // меняем ссылки соседних узлов
        if (OldNode->next) OldNode->next->prev = OldNode->prev; // если удаляемый элемент не последний, то мееняем ссылки соседних узлов
        else Tail = OldNode->prev; // если последний, то просто элемент перед OldNote - новый хвост
    }
    delete OldNode;
}
// проверка на пустоту
bool empty_list ()
{
    if (Head == NULL && Tail == NULL) return true;
    else return false;
}
// вывод листа
void print_list ()
{
    PNode q = Head;
    if (empty_list()) cout << "Лист пуст." << endl;
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
void filling_list_from_keyboard (int N)
{
    cout << "*** Осуществляется заполнение двусвязного списка значениями с клавиатуры ***" << endl;
    cout << "Задайте значения элементов двусвязного списка: " << endl;
    int chislo;
    PNode NewNode;
    for (int i = 0; i < N; i++)
    {
        cout << "Для " << i+1 << "-го: ";
        cin >> chislo;
        NewNode = CreateNode(chislo);
        AddLast(NewNode);
    }
}

void filling_list_random (int N, int min, int max)
{
    cout << "*** Осуществляется заполнение двусвязного списка случайными целыми числами ***" << endl;
    int chislo;
    PNode NewNode;
    int ostatok = (max - min) + 1;
    for (int i = 0; i < N; i++)
    {
        chislo = min + rand() % ( ostatok );
        NewNode = CreateNode(chislo);
        AddLast(NewNode);
    }
}

void filling_list_from_file (int N, string filename)
{
    cout << "*** Осуществляется заполнение двусвязного списка из файла " << filename << " ***" << endl;
    int chislo;
    PNode NewNode;
    ifstream input(filename);
    if (input) // проверка открытия
    {
        for (int i = 0; i < N; i++)
        {
            input >> chislo;
            NewNode = CreateNode(chislo);
            AddLast(NewNode);
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

void sposob_zapolneniya (int N)
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
        filling_list_from_keyboard(N);
        break;
    }
    case 2:
    {
        int min, max;
        diapazon_rand(min,max);
        filling_list_random(N,min,max);
        break;
    }
    case 3:
        string filename;
        cout << "Введите название файла: ";
        cin >> filename;
        filling_list_from_file(N,filename);
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

// Поиск первого квадрата в массиве
int first_full_square ()
{
    PNode q = Head;
    while (q && (!full_square(q->elem))) q = q->next;
    if (q == NULL) return -1;
    else return q->elem;
}

// Является ли число числом Фибоначчи
bool is_fibonachi (short int chislo)
{
    int summa(0), a1 (0), a2(1);
    while (summa < chislo)
    {
        summa = a1 + a2;
        a1 = a2;
        a2 = summa;
    }
    if (chislo == summa) return true;
    else return false;

}
//Поиск var - числа, в котором нет заданной цифры
bool find_var (short int chislo, short int zadannaya_cifra)
{
    if (chislo < 0) chislo = -chislo;
    short int ostatok;
    if (chislo == zadannaya_cifra) return false;
    while (chislo > 0)
    {
        ostatok = chislo % 10;
        if (ostatok == zadannaya_cifra) return false;
        chislo /= 10;
    }
    return true;
}

int insert_future_size_of_array (int N, short int zadannaya_cifra)
{
    int N_out = N;
    PNode q = Head;
    while (q)
    {
        if (find_var(q->elem,zadannaya_cifra))
        {
            N_out++;
        }
        q = q->next;
    }
    return N_out;
}

// Добавление первого полного квадрата после числа, в котором нет заданной цифры
void insert_after_var (int fullsquare, short int zadannaya_cifra)
{
    PNode q = Head;
    PNode NewNode;
    while (q)
    {
        if (find_var(q->elem,zadannaya_cifra))
        {
            NewNode = CreateNode(fullsquare);
            AddAfter(q, NewNode);
            q = q->next; // чтобы перескочить только что добавленный элемент
        }
        q = q->next;
    }
}
// Добавление до числа, в котором нет заданной цифры
void insert_before_var (int fullsquare, short int zadannaya_cifra)
{
    PNode q = Tail;
    PNode NewNode;
    while (q)
    {
        if (find_var(q->elem,zadannaya_cifra))
        {
            NewNode = CreateNode(fullsquare);
            AddBefore(q,NewNode);
           q = q->prev; // чтобы перескочить только что сдвинутый вправо var

        }
        q = q->prev;
    }
}

//Выясняем размер массива после удаления элементов
int delete_future_size_of_array (int N)
{
    PNode q = Head;
    int N_out = N;
    while (q)
    {
        if (is_fibonachi(q->elem)) N_out--;
        q = q->next;
    }
    return N_out;
}

// Удаление чисел Фибоначчи
void delete_fibonachi ()
{
    PNode q = Head; // для перебора списка
    PNode OldNode;
    while (q)
    {
        if (is_fibonachi(q->elem))
        {
            OldNode = q;
            q = q->next; // надо перейти к след элементу, так как сейчас адрес в q будет удален
            DeleteNode(OldNode);
        } else q = q->next;

    }
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

    // Ввод размера списка
    int N;
    cout << endl << "Задайте размер двусвязного списка: ";
    cin >> N;
    // Проверка на дурака
    while (N < 1)
    {
        cout << "error!" << endl;
        cout << "Задайте размер двусвязного списка: ";
        cin >> N;
    }
    // Конец ввода

    // Список (массив, вектор) заполняется один раз
    sposob_zapolneniya(N);
    cout << "Полученный двусвязный список: ";
    print_list();

    int menu;

    while (menu != 4 && N > 0)
    { //пока пользователь хочет продолжать работу или размер > 0 - работа продолжается
        display_menu(); // меню
        cout << "Выберите пункт меню: ";
        cin >> menu; // выбираем пункт меню и запоминаем номер
        //проверка на дурака
        while (menu < 0 || menu > 4)
        {
            cout << "error!" << endl;
            cout << "Выберите пункт меню: ";
            cin >> menu;
        }
        if (menu != 4) // завершает работу если пользователь выбрал 4 пункт меню
        {
            switch (menu) // переходим непосредственно к выполнению заданий
            {
                case 1:
                {
                    cout << "Выполнение задания 1.1 - Добавить первый элемент, являющийся полным квадратом после числа, в которых нет заданной цифры." << endl;
                    int add_elem = first_full_square(); // нахожу первый полный квадрат
                    if (add_elem == -1) // проверяю, нашлось ли оно
                    {

                        cout << "*** В двусвязном списке отсутствуют полные квадраты! ***" << endl;
                        cout << "Задайте число, которое необходимо добавить: ";
                        cin >> add_elem; // задаю вручную, если числа не нашлось

                    }

                    short int zadannaya_cifra;
                    cout << "Введите заданную цифру: ";
                    cin >> zadannaya_cifra; // ввожу заданную цифру
                    while (zadannaya_cifra < 0 || zadannaya_cifra > 9)
                    {
                        cout << "error!" << endl;
                        cout << "Введите заданную цифру: ";
                        cin >> zadannaya_cifra;
                    }

                    cout << " Односвязный список до изменений: ";
                    print_list();

                    int N_temp = insert_future_size_of_array(N,zadannaya_cifra); // вычисляю размер массива после добавления элемента
                    if (N_temp == N)
                    {
                        cout << "Односвязный список не подвергся изменениям, так как не было найдено числа, в котором нет заданной цифры." << endl;
                    }
                    else
                    {
                        insert_after_var(add_elem,zadannaya_cifra); // добавляю элементы в массив с помощью сдвига вправо
                        N = N_temp; // переопределяю размер
                        cout << " Односвязный список после изменений: ";
                        print_list();
                    }
                    cout << "Задание 1.1 выполнено!" << endl;
                    break;
                }
                case 2:
                {
                    cout << "Выполнение задания 1.2 - Добавить первый элемент, являющийся полным квадратом перед числом, в котором нет заданной цифры." << endl;
                    int add_elem = first_full_square(); // нахожу первый полный квадрат
                    if (add_elem == -1) // проверяю, нашлось ли оно
                    {

                        cout << "*** В двусвязном списке отсутствуют полные квадраты! ***" << endl;
                        cout << "Задайте число, которое необходимо добавить: ";
                        cin >> add_elem; // задаю вручную, если числа не нашлось

                    }

                    short int zadannaya_cifra;
                    cout << "Введите заданную цифру: ";
                    cin >> zadannaya_cifra; // ввожу заданную цифру
                    while (zadannaya_cifra < 0 || zadannaya_cifra > 9)
                    {
                        cout << "error!" << endl;
                        cout << "Введите заданную цифру: ";
                        cin >> zadannaya_cifra;
                    }

                    cout << " Односвязный список до изменений: ";
                    print_list();

                    int N_temp = insert_future_size_of_array(N,zadannaya_cifra); // вычисляю размер массива после добавления элемента
                    if (N_temp == N)
                    {
                        cout << "Односвязный список не подвергся изменениям, так как не было найдено числа, в котором нет заданной цифры." << endl;
                    }
                    else
                    {
                        insert_before_var(add_elem,zadannaya_cifra); // добавляю элементы в массив с помощью сдвига вправо
                        N = N_temp; // переопределяю размер
                        cout << " Односвязный список после изменений: ";
                        print_list();
                    }
                    cout << "Задание 1.2 выполнено!" << endl;
                    break;
                }
                case 3:
                {
                    cout << " Односвязный список до изменений: ";
                    print_list();
                    int N_temp = delete_future_size_of_array(N); // вычисляю размер массива после удаления чисел фибоначчи
                    if (N_temp == N)
                    {
                        cout << "Односвязный список не подвергся изменениям, так как не были найдены числа Фибоначчи." << endl;
                    }
                    else
                    {
                        delete_fibonachi(); // удаляю элементы с помощью сдвига влево
                        N = N_temp; // переопределяю размер N для A
                        cout << " Односвязный список после изменений: ";
                        print_list();
                    }
                    cout << "Задание 2 выполнено!" << endl;
                    break;
                }
            }
        }
    }
    if (N == 0) cout << "Односвязный список опустел в результате удаления элементов.";
    else cout << "Программа была завершена по желанию пользователя.";
    return 0;

}
