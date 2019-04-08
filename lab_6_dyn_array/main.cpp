// ver 1.2 - 08.04.19
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

using namespace std;

void filling_array_from_keyboard (short int A[], int N)
{
    cout << "*** Осуществляется заполнение динамического массива значениями с клавиатуры ***" << endl;
    cout << "Задайте значения элементов динамического массива: " << endl;
    int chislo;
    for (int i = 0; i < N; i++)
    {
        cout << "Для " << i+1 << "-го: ";
        cin >> chislo;
        A[i] = chislo;
    }
}

void filling_array_random (short int *A, int N, int min, int max)
{
    cout << "*** Осуществляется заполнение динамического массива случайными целыми числами ***" << endl;
    int chislo;
    int ostatok = (max - min) + 1;
    for (int i = 0; i < N; i++)
    {
        chislo = min + rand() % ( ostatok );
        A[i] = chislo;
    }
}

void filling_array_from_file (short int *A, int N, string filename)
{
    cout << "*** Осуществляется заполнение динамического массива из файла " << filename << " ***" << endl;
    int chislo;
    ifstream input(filename);
    if (input) // проверка открытия
    {
        for (int i = 0; i < N; i++)
        {
            input >> chislo;
            A[i] = chislo;
        }
        input.close();
    } else cout << endl << "error! Не удалось открыть файл " << filename << endl;
}

void print_array (const short int *A, int N)
{
    for (int i = 0; i < N; i++) cout << A[i] << " ";
    cout << endl;
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

void sposob_zapolneniya (short int *A, int N)
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
        filling_array_from_keyboard(A, N);
        break;
    }
    case 2:
    {
        int min, max;
        diapazon_rand(min,max);
        filling_array_random(A,N,min,max);
        break;
    }
    case 3:
        string filename;
        cout << "Введите название файла: ";
        cin >> filename;
        filling_array_from_file(A,N,filename);
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
short int first_full_square (const short int *A, int N)
{
    for(int i = 0; i < N; i++)
    {
        if (full_square( A[i] )) return A[i];
    }
    return -1; // значит полных квадратов в массиве нет
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
//Копирование массивов (из temp в A)
void Copy(const short int *temp, short int *A, int N)
{
    for (int i = 0; i < N; ++i)
        A[i] = temp[i];
}
//Выясняем размер массива после добавления элементов
int insert_future_size_of_array (const short int *A, int N, short int zadannaya_cifra)
{
    int N_out = N;
    for (int i = 0; i < N; i++)
    {
        if (find_var(A[i],zadannaya_cifra)) N_out++;
    }
    return N_out;
}
// Добавление первого полного квадрата после числа, в котором нет заданной цифры
void insert_after_var (const short int *A, short int *temp, int N, short int fullsquare, short int zadannaya_cifra)
{
    int i_temp = 0;
    for (int i = 0; i < N; i++)
    {
        temp[i_temp] = A[i];
        if (find_var(A[i],zadannaya_cifra))
        {
            i_temp++;
            temp[i_temp] = fullsquare;
        }
        i_temp++;
    }
}
// Добавление до числа, в котором нет заданной цифры
void insert_before_var (const short int *A, short int *temp, int N, short int fullsquare, short int zadannaya_cifra)
{
    int i_temp = 0;
    for (int i = 0; i < N; i++)
    {
        if (find_var(A[i],zadannaya_cifra))
        {
            temp[i_temp] = fullsquare;
            i_temp++;
            temp[i_temp] = A[i];
        }
        else temp[i_temp] = A[i];
        i_temp++;
    }
}
//Выясняем размер массива после удаления элементов
int delete_future_size_of_array (const short int *A, int N)
{
    int N_out = N;
    for (int i = 0; i < N; i++)
    {
        if (is_fibonachi(A[i]))
        {
            N_out--;
        }
    }
    return N_out;
}
// Удаление чисел Фибоначчи
void delete_fibonachi (const short int *A, short int *temp, int N)
{
    int i_temp = 0;
    for (int i = 0; i < N; i++)
    {
        if (!is_fibonachi(A[i]))
        {
            temp[i_temp] = A[i];
            i_temp++;
        }
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

    // Ввод размера массива
    int N;
    cout << endl << "Задайте размер динамического массива: ";
    cin >> N;
    // Проверка на дурака
    while (N < 1)
    {
        cout << "error!" << endl;
        cout << "Задайте размер динамического массива: ";
        cin >> N;
    }
    // Конец ввода

    short int *A = new short int[N]; // объявляю указатель на рабочий массив и выделяю для него размер N
    short int *temp; // объявляю указатель на временный массив

    // Список (массив, вектор) заполняется один раз
    sposob_zapolneniya(A,N);
    cout << "Полученный динамический массив: ";
    print_array(A,N);

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
                    short int add_elem = first_full_square(A,N); // нахожу первый полный квадрат
                    if (add_elem == -1) // проверяю, нашлось ли оно
                    {

                        cout << "*** В динамическом массиве отсутствуют полные квадраты! ***" << endl;
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

                    cout << " Динамический массив до изменений: ";
                    print_array(A,N);

                    int N_temp = insert_future_size_of_array(A,N,zadannaya_cifra); // вычисляю размер массива после добавления элемента
                    if (N_temp == N)
                    {
                        cout << "Динамический не подвергся изменениям, так как не было найдено числа, в котором нет заданной цифры." << endl;
                    }
                    else
                    {
                        temp = new short int[N_temp]; // выделяю память под массив temp
                        insert_after_var(A,temp,N,add_elem,zadannaya_cifra); // переписываю все элементы массива A в массив Temp и заодно добавляю нужные элементы
                        delete[] A; // очищаю память выделенную для A
                        N = N_temp; // переопределяю размер N для A
                        A = new short int[N]; // выделяю память под уже новый массив A
                        Copy(temp,A,N); // копирую элементы из массива temp в массив A
                        delete[] temp; // очищаю память выделенную под temp
                        cout << " Динамический массив после изменений: ";
                        print_array(A,N);
                    }
                    cout << "Задание 1.1 выполнено!" << endl;
                    break;
                }
                case 2:
                {
                    cout << "Выполнение задания 1.2 - Добавить первый элемент, являющийся полным квадратом перед числом, в котором нет заданной цифры." << endl;
                    short int add_elem = first_full_square(A,N); // нахожу первый полный квадрат
                    if (add_elem == -1) // проверяю, нашлось ли оно
                    {

                        cout << "*** В динамическом массиве отсутствуют полные квадраты! ***" << endl;
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

                    cout << " Динамический массив до изменений: ";
                    print_array(A,N);

                    int N_temp = insert_future_size_of_array(A,N,zadannaya_cifra); // вычисляю размер массива после добавления элемента
                    if (N_temp == N)
                    {
                        cout << "Динамический не подвергся изменениям, так как не было найдено числа, в котором нет заданной цифры." << endl;
                    }
                    else
                    {
                        temp = new short int[N_temp]; // выделяю память под массив temp
                        insert_before_var(A,temp,N,add_elem,zadannaya_cifra); // переписываю все элементы массива A в массив Temp и заодно добавляю нужные элементы
                        delete[] A; // очищаю память выделенную для A
                        N = N_temp; // переопределяю размер N для A
                        A = new short int[N]; // выделяю память под уже новый массив A
                        Copy(temp,A,N); // копирую элементы из массива temp в массив A
                        delete[] temp; // очищаю память выделенную под temp
                        cout << " Динамический массив после изменений: ";
                        print_array(A,N);
                    }
                    cout << "Задание 1.2 выполнено!" << endl;
                    break;
                }
                case 3:
                {
                    cout << " Динамический массив до изменений: ";
                    print_array(A,N);
                    int N_temp = delete_future_size_of_array(A,N); // вычисляю размер массива после удаления чисел фибоначчи
                    if (N_temp == N)
                    {
                        cout << "Динамический массив не подвергся изменениям, так как не были найдены числа Фибоначчи." << endl;
                    }
                    else
                    {
                        temp = new short int[N_temp]; // выделяю память под массив temp
                        delete_fibonachi(A,temp,N); // переписываю все элементы массива А в массив temp, кроме чисел фибоначчи
                        delete[] A; // очищаю память выделенную для A
                        N = N_temp; // переопределяю размер N для A
                        A = new short int[N]; // выделяю память под уже новый массив A
                        Copy(temp,A,N); // копирую элементы из массива temp в массив A
                        delete[] temp; // очищаю память выделенную под temp
                        cout << " Динамический массив после изменений: ";
                        print_array(A,N);
                    }
                    cout << "Задание 2 выполнено!" << endl;
                    break;
                }
            }
        }
    }
    if (N == 0) cout << "Динамический массив опустел в результате удаления элементов.";
    else cout << "Программа была завершена по желанию пользователя.";
    return 0;

}
