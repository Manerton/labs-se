// ver 1.3 - 02.04.19 -- Вектор готов
#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <cmath>

using namespace std;

vector<short int> filling_vector_from_keyboard (int N)
{
    vector<short int> A(N);
    cout << "*** Осуществляется заполнение вектора значениями с клавиатуры ***" << endl;
    cout << "Задайте значения элементов вектора: " << endl;
    int chislo;
    for (int i = 0; i < N; i++)
    {
        cout << "Для " << i+1 << "-го: ";
        cin >> chislo;
        A[i] = chislo;
    }
    return A;
}

vector<short int> filling_vector_random (int N, int min, int max)
{
    vector<short int> A(N);
    cout << "*** Осуществляется заполнение вектора случайными целыми числами ***" << endl;
    int chislo;
    int ostatok = (max - min) + 1;
    for (int i = 0; i < N; i++)
    {
        chislo = min + rand() % ( ostatok );
        A[i] = chislo;
    }
    return A;
}

vector<short int> filling_vector_from_file (int N, string filename)
{
    vector<short int> A(N);
    cout << "*** Осуществляется заполнение вектора из файла " << filename << " ***" << endl;
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
    return A;
}

void print_array (const vector<short int> &A)
{
    for (vector<short int>::const_iterator i = A.begin(); i != A.end(); ++i) cout << *i << " ";
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

vector<short int> sposob_zapolneniya (int N)
{
    vector<short int> A;
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
        A = filling_vector_from_keyboard(N);
        break;
    }
    case 2:
    {
        int min, max;
        diapazon_rand(min,max);
        A = filling_vector_random(N,min,max);
        break;
    }
    case 3:
        string filename;
        cout << "Введите название файла: ";
        cin >> filename;
        A = filling_vector_from_file(N,filename);
        break;
    }
    return A;
}

//Проверка на полный квадрат (если корень double числа равен корню приведенному к int, то число - полный квадрат)
bool full_square (short int chislo)
{
    double sqrt_chislo = sqrt(chislo);
    if (chislo >= 0 && sqrt_chislo == static_cast<short int> (sqrt_chislo)) return true;
    else return false;
}

short int first_full_square (const vector<short int> &A)
{
    unsigned int N = A.size();
    for(unsigned int i = 0; i != N; i++)
    {
        if (full_square( A[i] )) return A[i];
    }
    return -1; // значит полных квадратов в векторе нет
}

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
    if (chislo == zadannaya_cifra) return true;
    while (chislo > 0)
    {
        ostatok = chislo % 10;
        if (ostatok == zadannaya_cifra) return true;
        chislo /= 10;
    }
    return false;
}

// Добавление первого полного квадрата после числа, в котором нет заданной цифры
void insert_after_var (vector<short int> &A, short int fullsquare, short int zadannaya_cifra)
{
    for (unsigned int i = 0; i != A.size(); i++)
    {
        if (!find_var(A[i],zadannaya_cifra))
        {
            A.reserve(A.size()+1);
            A.insert(A.begin()+i+1,fullsquare);
            i++; // чтобы перескочить только что найденный элемент i
        }
    }
}
// Добавление до числа, в котором нет заданной цифры
void insert_before_var (vector<short int> &A, short int fullsquare, short int zadannaya_cifra)
{
    for (unsigned int i = 0; i != A.size(); i++)
    {
        if (!find_var(A[i],zadannaya_cifra))
        {
            A.reserve(A.size()+1);
            A.insert(A.begin()+i,fullsquare);
            i++; // чтобы перескочить только что найденный элемент i
        }
    }
}

// Удаление чисел Фибоначчи
void delete_fibonachi (vector<short int> &A)
{
    for (unsigned int i = 0; i != A.size(); i++)
    {
        if (is_fibonachi(A[i]))
        {
            A.erase(A.begin()+i);
            i--; // так как элемент под i был удален и теперь на этом месте другой элемент
        }
    }
}
int main()
{
    srand((unsigned)time(0));
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №6 Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
    bool FirstTime = true;
    vector<short int> A;
    unsigned int N;
    int menu;
    while (menu != 4 && N > 0) { //пока пользователь хочет продолжать работу или размер > 0 - работа продолжается
        //меню
        cout << "*** Меню ***" << endl;
        cout << "1. Задание 1.1 - Добавить первый элемент, являющийся полным квадратом после числа, в которых нет заданной цифры." << endl;
        cout << "2. Задание 1.2 - Добавить первый элемент, являющийся полным квадратом перед числом, в котором нет заданной цифры." << endl;
        cout << "3. Задание 2 - Удалить все числа Фибоначчи." << endl;
        cout << "4. Завершить работу программы." << endl;
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
            if (FirstTime) // при первом запуске программы, после меню должно выйти предложение с заполнением массива, поэтому используем флаг
            {
                cout << "Задайте размер вектора: ";
                cin >> N;
                while (N < 1)
                {
                    cout << "error!" << endl;
                    cout << "Задайте размер вектора: ";
                    cin >> N;
                }
                A = sposob_zapolneniya(N); // Список (массив, вектор) заполняется один раз
                FirstTime = false;
            }
            switch (menu) // переходим непосредственно к выполнению заданий
            {
                case 1:
                {
                    cout << "Выполнение задания 1.1 - Добавить первый элемент, являющийся полным квадратом после числа, в которых нет заданной цифры." << endl;
                    short int add_elem = first_full_square(A);
                    short int zadannaya_cifra;
                    if (add_elem == -1)
                    {

                        cout << "*** В векторе отсутствуют полные квадраты! ***" << endl;
                        cout << "Задайте число, которое необходимо добавить: ";
                        cin >> add_elem;
                    }

                    cout << "Введите заданную цифру: ";
                    cin >> zadannaya_cifra;
                    while (zadannaya_cifra < 0 || zadannaya_cifra > 9)
                    {
                        cout << "error!" << endl;
                        cout << "Введите заданную цифру: ";
                        cin >> zadannaya_cifra;
                    }
                    cout << " Вектор до изменений: ";
                    print_array(A);

                    /// Выполнение задания
                    insert_after_var(A,add_elem,zadannaya_cifra);
                    if (A.size() == N)
                    {
                        cout << "Вектор не подвергся изменениям, так как не было найдено числа, в котором нет заданной цифры." << endl;
                    }
                    else
                    {
                        cout << " Вектор после изменений: ";
                        print_array(A);
                        N = A.size();
                    }
                    cout << "Задание 1.1 выполнено!" << endl;
                    break;
                }
                case 2:
                {
                    cout << "Выполнение задания 1.2 - Добавить первый элемент, являющийся полным квадратом перед числом, в котором нет заданной цифры." << endl;
                    short int add_elem = first_full_square(A);
                    short int zadannaya_cifra;
                    if (add_elem == -1)
                    {

                        cout << "*** В векторе отсутствуют полные квадраты! ***" << endl;
                        cout << "Задайте число, которое необходимо добавить: ";
                        cin >> add_elem;
                    }

                    cout << "Введите заданную цифру: ";
                    cin >> zadannaya_cifra;
                    while (zadannaya_cifra < 0 || zadannaya_cifra > 9)
                    {
                        cout << "error!" << endl;
                        cout << "Введите заданную цифру: ";
                        cin >> zadannaya_cifra;
                    }
                    cout << " Вектор до изменений: ";
                    print_array(A);

                    /// Выполнение задания
                    insert_before_var(A,add_elem,zadannaya_cifra);
                    if (A.size() == N)
                    {
                        cout << "Вектор не подвергся изменениям, так как не было найдено числа, в котором нет заданной цифры." << endl;
                    }
                    else
                    {
                        cout << " Вектор после изменений: ";
                        print_array(A);
                        N = A.size();
                    }
                    cout << "Задание 1.2 выполнено!" << endl;
                    break;
                }
                case 3:
                {
                    cout << " Вектор до изменений: ";
                    print_array(A);
                    delete_fibonachi(A);
                    if (A.size() == N)
                    {
                        cout << "Вектор не подвергся изменениям, так как не были найдены числа Фибоначчи." << endl;
                    }
                    else
                    {
                        cout << " Вектор после изменений: ";
                        print_array(A);
                        N = A.size();
                    }
                    cout << "Задание 2 выполнено!" << endl;
                    break;
                }
            }
        }
    }
    if (N == 0) cout << "Вектор опустел в результате удаления элементов.";
    else cout << "Программа была завершена по желанию пользователя.";

    return 0;

}
