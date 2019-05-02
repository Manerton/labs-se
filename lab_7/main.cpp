//ver 1 - 02.05.19

#include <iostream>
#include <fstream>
#include <ctime>
#include <array>
#include <vector>
#include <cmath>

using namespace std;
// M - Строки, N - Столбцы
const int size = 10;

//Инициализация для array
void init_random(array<array<int, size>, size> &A, int M, int N, const int &min, const int &max)
{
    cout << "*** Осуществляется заполнение матрицы случайными целыми числами ***" << endl;
    int ostatok = (max - min) + 1;
    for(int i=0; i < M; i++)
    {
        for(int j=0; j < N; j++)
        {
            A[i][j]= min + rand() % ( ostatok );
        }
    }

}

// Инициализация для двумерного массива фикс размера
void init_random(int A[size][size], int M, int N, const int &min, const int &max)
{
    cout << "*** Осуществляется заполнение матрицы случайными целыми числами ***" << endl;
    int ostatok = (max - min) + 1;
    for(int i=0; i < M; i++)
    {
        for(int j=0; j < N; j++)
        {
            A[i][j]= min + rand() % ( ostatok );
        }
    }

}

// Инициализация для вектора
void init_random(vector<vector<int>> &A, int M, int N, const int &min, const int &max)
{
    cout << "*** Осуществляется заполнение матрицы случайными целыми числами ***" << endl;
    int ostatok = (max - min) + 1;
    for(int i=0; i < M; i++)
    {
        for(int j=0; j < N; j++)
        {
            A[i][j]= min + rand() % ( ostatok );
        }
    }

}

// Инициализация для двумерного дин массива
void init_random(int **A, int M, int N, const int &min, const int &max)
{
    cout << "*** Осуществляется заполнение матрицы случайными целыми числами ***" << endl;
    int ostatok = (max - min) + 1;
    for(int i=0; i < M; i++)
    {
        for(int j=0; j < N; j++)
        {
            A[i][j]= min + rand() % ( ostatok );
        }
    }

}

// Проверка на кол-во чисел в файле
bool checkfile(int M, int N)
{
    ifstream input("input.txt");
    int i=0, temp;
    int count = M * N;
    if (input) // проверка открытия
    {
        while (i < count && !input.eof())
        {
            input >> temp;
            i++;
        }
        input.close();
    } else cout << endl << "error! Не удалось открыть файл input.txt"<< endl;
    return (i == count);
}

// Инициализация из файла для array
void init_from_file(array<array<int, size>, size> &A, int M, int N)
{
    cout << "*** Осуществляется заполнение матрицы из файла input.txt ***" << endl;
    ifstream input("input.txt");
    if (input) // проверка открытия
    {
        for(int i=0; i < M; i++)
        {
            for(int j=0; j < N; j++)
            {
                input >> A[i][j];
            }
        }
        input.close();
    } else cout << endl << "error! Не удалось открыть файл input.txt"<< endl;
}

// Инициализация из файла для двумерного массива фикс размера
void init_from_file(int A[size][size], int M, int N)
{
    cout << "*** Осуществляется заполнение матрицы из файла input.txt ***" << endl;
    ifstream input("input.txt");
    if (input) // проверка открытия
    {
        for(int i=0; i < M; i++)
        {
            for(int j=0; j < N; j++)
            {
                input >> A[i][j];
            }
        }
        input.close();
    } else cout << endl << "error! Не удалось открыть файл input.txt"<< endl;
}

// Инициализация из файла для вектора
void init_from_file(vector<vector<int>> &A, int M, int N)
{
    cout << "*** Осуществляется заполнение матрицы из файла input.txt ***" << endl;
    ifstream input("input.txt");
    if (input) // проверка открытия
    {
        for(int i=0; i < M; i++)
        {
            for(int j=0; j < N; j++)
            {
                input >> A[i][j];
            }
        }
        input.close();
    } else cout << endl << "error! Не удалось открыть файл input.txt"<< endl;
}

// Инициализация из файла для двумерного дин массива
void init_from_file(int **A, int M, int N)
{
    cout << "*** Осуществляется заполнение матрицы из файла input.txt ***" << endl;
    ifstream input("input.txt");
    if (input) // проверка открытия
    {
        for(int i=0; i < M; i++)
        {
            for(int j=0; j < N; j++)
            {
                input >> A[i][j];
            }
        }
        input.close();
    } else cout << endl << "error! Не удалось открыть файл input.txt"<< endl;
}

// Вывод для array
void display (const array<array<int, size>, size> &A, int M, int N)
{
    for(int i=0; i < M; i++)
    {
        for(int j=0; j < N; j++)
        {
            cout << A[i][j] << "\t";
        }
        cout << endl;
    }
}

// Вывод для двумерного массива фикс размера
void display (const int A[size][size], int M, int N)
{
    for(int i=0; i < M; i++)
    {
        for(int j=0; j < N; j++)
        {
            cout << A[i][j] << "\t";
        }
        cout << endl;
    }
}

// Вывод для вектора
void display (const vector<vector<int>> &A, int M, int N)
{
    for(int i=0; i < M; i++)
    {
        for(int j=0; j < N; j++)
        {
            cout << A[i][j] << "\t";
        }
        cout << endl;
    }
}

// Вывод для двумерного дин массива
void display (int **A, int M, int N)
{
    for(int i=0; i < M; i++)
    {
        for(int j=0; j < N; j++)
        {
            cout << A[i][j] << "\t";
        }
        cout << endl;
    }
}

// Вывод одномерного массива vector
void display (const vector<int> &A, int N)
{
    for(int i=0; i < N; i++)
    {
        cout << A[i] << "\t";
    }
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

// Способы заполнения для array
void sposob_zapolneniya (array<array<int, size>, size> &A, int M, int N)
{
    int choice;
    cout << "*** Выбор способа заполнения ***" << endl;
    cout << "1. Заполнение случайными числами." << endl;
    cout << "2. Заполнение из файла." << endl;
    cout << "Выберите способ заполнения: ";
    cin >> choice;
    while (choice < 1 || choice > 2)
    {
        cout << "error!" << endl;
        cout << "Выберите способ заполнения: ";
        cin >> choice;
    }
    switch (choice)
    {
    case 1:
    {
        int min, max;
        diapazon_rand(min,max);
        init_random(A,M,N,min,max);
        break;
    }
    case 2:
        if (checkfile(M,N)) init_from_file(A,M,N);
        else {
            cout << "В файле недостаточно чисел!" << endl;
            sposob_zapolneniya(A,M,N);
        }
        break;
    }
}

// Способы заполнения для двумерного массива фикс размера
void sposob_zapolneniya (int A[size][size], int M, int N)
{
    int choice;
    cout << "*** Выбор способа заполнения ***" << endl;
    cout << "1. Заполнение случайными числами." << endl;
    cout << "2. Заполнение из файла." << endl;
    cout << "Выберите способ заполнения: ";
    cin >> choice;
    while (choice < 1 || choice > 2)
    {
        cout << "error!" << endl;
        cout << "Выберите способ заполнения: ";
        cin >> choice;
    }
    switch (choice)
    {
    case 1:
    {
        int min, max;
        diapazon_rand(min,max);
        init_random(A,M,N,min,max);
        break;
    }
    case 2:
        if (checkfile(M,N)) init_from_file(A,M,N);
        else {
            cout << "В файле недостаточно чисел!" << endl;
            sposob_zapolneniya(A,M,N);
        }

        break;
    }
}

// Инициализация пустого вектора M x N
void init_empty_vector (vector<vector<int>> &A, int M, int N)
{
    vector<int> Row(N,0);
    for (int i=0; i < M; i++)
    {
        A.push_back(Row);
    }
}

// Способы заполнения для vector
void sposob_zapolneniya (vector<vector<int>> &A, int M, int N)
{
    int choice;
    cout << "*** Выбор способа заполнения ***" << endl;
    cout << "1. Заполнение случайными числами." << endl;
    cout << "2. Заполнение из файла." << endl;
    cout << "Выберите способ заполнения: ";
    cin >> choice;
    while (choice < 1 || choice > 2)
    {
        cout << "error!" << endl;
        cout << "Выберите способ заполнения: ";
        cin >> choice;
    }
    init_empty_vector(A, M, N);
    switch (choice)
    {
    case 1:
    {
        int min, max;
        diapazon_rand(min,max);
        init_random(A,M,N,min,max);
        break;
    }
    case 2:
        if (checkfile(M,N)) init_from_file(A,M,N);
        else {
            cout << "В файле недостаточно чисел!" << endl;
            sposob_zapolneniya(A,M,N);
        }
        break;
    }
}

// Способы заполнения для двумерного дин массива
void sposob_zapolneniya (int **A, int M, int N)
{
    int choice;
    cout << "*** Выбор способа заполнения ***" << endl;
    cout << "1. Заполнение случайными числами." << endl;
    cout << "2. Заполнение из файла." << endl;
    cout << "Выберите способ заполнения: ";
    cin >> choice;
    while (choice < 1 || choice > 2)
    {
        cout << "error!" << endl;
        cout << "Выберите способ заполнения: ";
        cin >> choice;
    }
    switch (choice)
    {
    case 1:
    {
        int min, max;
        diapazon_rand(min,max);
        init_random(A,M,N,min,max);
        break;
    }
    case 2:
        if (checkfile(M,N)) init_from_file(A,M,N);
        else {
            cout << "В файле недостаточно чисел!" << endl;
            sposob_zapolneniya(A,M,N);
        }

        break;
    }
}

// Четное или нет
bool is_chetnoe (int elem)
{
    return (elem % 2 == 0);
}

//Проверка на полный квадрат (если корень double числа равен корню приведенному к int, то число - полный квадрат)
bool full_square (int chislo)
{
    double sqrt_chislo = sqrt(chislo);
    if (chislo >= 0 && sqrt_chislo == static_cast<int> (sqrt_chislo)) return true;
    else return false;
}

// Первое задания для Array
void ex1 (const array<array<int, size>, size> &A, int M, int N)
{
    vector<int> Arr(N, 0);
    for (int i = 0; i < N; i++)
    {
        int j = 0;
        Arr[i] = 1;
        while (j < M)
        {
            if (is_chetnoe(A[j][i]))
            {
                Arr[i] = A[j][i];
                j = M;
            }
            j++;
        }
    }
    cout << "Вывод сформированного массива: ";
    display(Arr, N);
}

//Первое задание для двумерного массива фикс размера
void ex1 (const int A[size][size], int M, int N)
{
    vector<int> Arr(N,0);
    for (int i = 0; i < N; i++)
    {
        int j = 0;
        Arr[i] = 1;
        while (j < M)
        {
            if (is_chetnoe(A[j][i]))
            {
                Arr[i] = A[j][i];
                j = M;
            }
            j++;
        }
    }
    cout << "Вывод сформированного массива: ";
    display(Arr,N);
}

// Второе задание для Array
int ex2 (const array<array<int, size>, size> &A, int M, int N)
{
    for(int i = 0; i < M; i++)
    {
        int count = 0;
        for(int j = 0; j < N; j++)
        {
            if (full_square(A[i][j])) count++;
        }
        if (count == N) return i+1;
    }
    return -1;
}

//Второе задание для двумерного массива фикс размера
int ex2 (const int A[size][size], int M, int N)
{
    for(int i = 0; i < M; i++)
    {
        int count = 0;
        for(int j = 0; j < N; j++)
        {
            if (full_square(A[i][j])) count++;
        }
        if (count == N) return i+1;
    }
    return -1;
}
// Для array
int index_max_elem (array<int, size> &row, int N)
{
    int max = row[0];
    int i_max = 0;
    for (int i = 1; i < N; i++)
    {
        if (row[i] > max)
        {
            max = row[i];
            i_max = i;
        }
    }
    return i_max;
}

// Для вектора
int index_max_elem (vector<int> &row, int N)
{
    int max = row[0];
    int i_max = 0;
    for (int i = 1; i < N; i++)
    {
        if (row[i] > max)
        {
            max = row[i];
            i_max = i;
        }
    }
    return i_max;
}

// Для дин массива
int index_max_elem (int *row, int N)
{
    int max = row[0];
    int i_max = 0;
    for (int i = 1; i < N; i++)
    {
        if (row[i] > max)
        {
            max = row[i];
            i_max = i;
        }
    }
    return i_max;
}

// Третье задание для Array
void ex3 (array<array<int, size>, size> &A, int M, int N)
{
    for(int i = 0; i < M; i++)
    {
        int i_max = index_max_elem(A[i],N);
        int summa = 0;
        for (int j = 0; j < i_max; j++)
        {
            summa += A[i][j];
        }
        if (i_max != 0) A[i][i_max] = summa;
    }
    for (int j = 0; j < N-1; j++)
    {
        for (int i = 0; i < M; i++)
        {
            int temp;
            temp = A[i][j];
            A[i][j] = A[i][j+1];
            A[i][j+1] = temp;
        }
        j++;
    }
}

// Для вектора задание 3
void ex3 (vector<vector<int>> &A, int M, int N)
{
    for(int i = 0; i < M; i++)
    {
        int i_max = index_max_elem(A[i], N);
        int summa = 0;
        for (int j = 0; j < i_max; j++)
        {
            summa += A[i][j];
        }
        if (i_max != 0) A[i][i_max] = summa;
    }
    for (int j = 0; j < N-1; j++)
    {
        for (int i = 0; i < M; i++)
        {
            int temp;
            temp = A[i][j];
            A[i][j] = A[i][j+1];
            A[i][j+1] = temp;
        }
        j++;
    }
}

// Третье задание для дин массива
void ex3 (int **A, int M, int N)
{
    for(int i = 0; i < M; i++)
    {
        int i_max = index_max_elem(A[i],N);
        int summa = 0;
        for (int j = 0; j < i_max; j++)
        {
            summa += A[i][j];
        }
        if (i_max != 0) A[i][i_max] = summa;
    }
    for (int j = 0; j < N-1; j++)
    {
        for (int i = 0; i < M; i++)
        {
            int temp;
            temp = A[i][j];
            A[i][j] = A[i][j+1];
            A[i][j+1] = temp;
        }
        j++;
    }
}

void find_min_elem (vector<vector<int>> &A, int M, int N, int &i_min, int &j_min)
{
    int min = A[0][0];
    i_min = 0, j_min = 0;
    for (int i = 0; i < M; i++)
    {
        for(int j = 0; j < N; j++)
        {
            if (A[i][j] < min)
            {
                min = A[i][j];
                i_min = i;
                j_min = j;
            }
        }
    }
}

int find_max_elem (vector<vector<int>> &A, int M, int N)
{
    int max = A[0][0];
    for (int i = 0; i < M; i++)
    {
        for(int j = 0; j < N; j++)
        {
            if (A[i][j] > max)
            {
                max = A[i][j];
            }
        }
    }
    return max;
}

int find_max_elem (int **A, int M, int N)
{
    int max = A[0][0];
    for (int i = 0; i < M; i++)
    {
        for(int j = 0; j < N; j++)
        {
            if (A[i][j] > max)
            {
                max = A[i][j];
            }
        }
    }
    return max;
}

void find_min_elem (int **A, int M, int N, int &i_min, int &j_min)
{
    int min = A[0][0];
    i_min = 0, j_min = 0;
    for (int i = 0; i < M; i++)
    {
        for(int j = 0; j < N; j++)
        {
            if (A[i][j] < min)
            {
                min = A[i][j];
                i_min = i;
                j_min = j;
            }
        }
    }
}

// Для вектора - задание 4
void ex4 (vector<vector<int>> &A, int &M, int &N)
{
    int i_min, j_min, chislo;
    find_min_elem(A,M,N,i_min,j_min);
    cout << "Введите заданное число: " << endl;
    cin >> chislo;
    vector<int> insertRow(N,chislo);
    A.insert(A.begin()+i_min, insertRow);
    M++;
    for (int i = 0; i < M; i++)
    {
        A[i].insert(A[i].begin()+j_min, 0);
    }
    N++;
}

void ex4 (int **Matrix, int **Matrix_temp, int M, int N)
{
    int i_min, j_min, chislo;
    int M_temp = M+1;
    int i_temp = 0;
    find_min_elem(Matrix,M,N,i_min,j_min);
    cout << "Введите заданное число: " << endl;
    cin >> chislo;
    for (int i = 0; i < M; i++)
    {
        if (i != i_min)
        {
            for (int j = 0; j < N; j++)
            {
                Matrix_temp[i_temp][j] = Matrix[i][j];
            }
        } else {
            for (int j = 0; j < N; j++)
            {
                Matrix_temp[i_temp][j] = chislo;
                Matrix_temp[i_temp+1][j] = Matrix[i][j];
            }
            i_temp++; // чтобы пропустить только что добавленную строку
        }
        i_temp++;
    }
    for (int i = 0; i < M_temp; i++)
    {
        for (int j = N; j != j_min; j--)
        {
            Matrix_temp[i][j+1] = Matrix_temp[i][j];
        }
        Matrix_temp[i][j_min] = 0;
    }
}

int sposob_realisacii (int menu)
{
    int sposob;
    if (menu < 3) menu = 1;
    if (menu > 3) menu = 5;
    cout << "Выберите способ реализации матрицы: " << endl;
    switch (menu)
    {
        case 1:
        {
            cout << "1) двумерный массив фиксированного размера." << endl;
            cout << "2) матрица реализована с использованием библиотеки STL (класс array)." << endl;
            cin >> sposob;
            while (sposob < 1 || sposob > 2)
            {
                cout << "error!" << endl;
                cout << "Выберите способ реализации матрицы: ";
            }
            break;
        }
        case 3:
        {
             cout << "1) матрица реализована с использованием библиотеки STL (класс array)." << endl;
             cout << "2) динамический двумерный массив (массив указателей на строки)." << endl;
             cout << "3) матрица реализована с использованием библиотеки STL (класс vector)." << endl;
             cin >> sposob;
             while (sposob < 1 || sposob > 3)
             {
                cout << "error!" << endl;
                cout << "Выберите способ реализации матрицы: ";
             }
             break;
        }
        case 5:
        {
            cout << "1) динамический двумерный массив (массив указателей на строки)." << endl;
            cout << "2) матрица реализована с использованием библиотеки STL (класс vector)." << endl;
            cin >> sposob;
            while (sposob < 1 || sposob > 2)
            {
                cout << "error!" << endl;
                cout << "Выберите способ реализации матрицы: ";
            }
            break;
        }
    }
    return sposob;
}

bool more_one_max (vector<int> &row, int N, int max)
{
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        if (row[i] == max) count++;
        if (count > 1) return true;
    }
    return false;
}

bool more_one_max (int *row, int N, int max)
{
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        if (row[i] == max) count++;
        if (count > 1) return true;
    }
    return false;
}

bool summa_in_col (vector<vector<int>> &A, int M, int j, int chislo)
{
    int summa = 0;
    for (int i = 0; i < M; i++)
    {
        summa += A[i][j];
    }
    return (chislo == summa);
}

bool summa_in_col (int **A, int M, int j, int chislo)
{
    int summa = 0;
    for (int i = 0; i < M; i++)
    {
        summa += A[i][j];
    }
    return (chislo == summa);
}

void ex5 (vector<vector<int>> &A, int &M, int &N)
{
    int max = find_max_elem(A,M,N);
    for (int i = 0; i < M; i++)
    {
        if (more_one_max(A[i], N, max))
        {
            A.erase(A.begin()+i);
            M--;
            i--;
        }
    }
    int chislo;
    cout << "Введите заданное число: ";
    cin >> chislo;
    for (int j = 0; j < N; j++)
    {
        if (summa_in_col(A,M,j,chislo))
        {
            for (int i = 0; i < M; i++)
            {
                A[i].erase(A[i].begin()+j);
            }
            N--;
            j--;
        }
    }
}

void ex5_1 (int **Matrix, int **Matrix_temp, int M, int N)
{
    int max = find_max_elem(Matrix,M,N);
    int i_temp = 0;
    for (int i = 0; i < M; i++)
    {
        if (!more_one_max(Matrix[i], N, max))
        {
            for (int j = 0; j < N; j++)
            {
                Matrix_temp[i_temp][j] = Matrix[i][j];
            }
         i_temp++;
        }
    }
}

void ex5_2 (int **Matrix_temp, int **Matrix_new, int M, int N, int chislo)
{
    int j_temp = 0;
    for (int j = 0; j < N; j++)
    {
        if (!summa_in_col(Matrix_temp,M,j,chislo))
        {
            for (int i = 0; i < M; i++)
            {
                Matrix_new[i][j_temp] = Matrix_temp[i][j];
            }
        j_temp++;
        }
    }
}

void new_size_M (int **A, int &M_new, int N)
{
    int max = find_max_elem(A,M_new,N);
    int M = M_new;
    for (int i = 0; i < M; i++)
    {
        if (more_one_max(A[i], N, max))
        {
            M_new--;
        }
    }
}

void new_size_N (int **A, int M, int &N_new, int chislo)
{
    int N = N_new;
    for (int j = 0; j < N; j++)
    {
        if (summa_in_col(A,M,j,chislo))
        {
            N_new--;
        }
    }
}


void display_menu ()
{
    cout << endl;
    cout << "*** Меню ***" << endl;
    cout << "1) Дана матрица размером M x N. Сформировать одномерный массив, каждый элемент которого равен первому четному элементу соответствующего столбца, если такого нет, то равен единице." << endl;
    cout << "2) Дана матрица размером M x N. Определить, есть ли в данной матрице строка, в которой все числа являются полными квадратами, если есть – сообщить её номер." << endl;
    cout << "3) Дана матрица размером M x N." << endl;
    cout << " 3.1) Заменить максимальный элемент каждой строки суммой всех предыдущих элементов этой строки." << endl;
    cout << " 3.2) Поменять местами первый и второй, третий и четвёртый и т.д. столбцы." << endl;
    cout << "4) Вставить строку из элементов, равных заданному числу и столбец из нулей перед строкой и столбцом, в которых встретился минимальный элемент." << endl;
    cout << "5) Удалить строки, содержащие более одного максимального элемента, а затем столбцы, сумма элементов которых равна заданному числу. Если такого столбца или строки нет, то вывести сообщение об этом." << endl;
    cout << "6) Завершить работу программы." << endl;
}

int main()
{
    srand((unsigned)time(0));
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №7 Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
    int menu(0), sposob;
    while (menu != 6)
    {
        display_menu();
        cout << "Выберите пункт меню: ";
        cin >> menu; // выбираем пункт меню и запоминаем номер
        //проверка на дурака
        while (menu < 1 || menu > 6)
        {
            cout << "error!" << endl;
            cout << "Выберите пункт меню: ";
            cin >> menu;
        }
        if (menu != 6)
        {
            // Ввод M и N
            int M, N;
            cout << "Задайте количество строк M: ";
            cin >> M;
            while (M < 1)
            {
                cout << "error!" << endl;
                cout << "Задайте количество строк M: ";
                cin >> M;
            }
            cout << "Задайте количество столбцов N: ";
            cin >> N;
            while (N < 1)
            {
                cout << "error!" << endl;
                cout << "Задайте количество столбцов N: ";
                cin >> N;
            }
            // Конец ввода

            sposob = sposob_realisacii(menu);
            switch (menu) // переходим непосредственно к выполнению заданий
            {
            case 1:
            {
                 cout << "Выполнения задания 1." << endl;

                 switch (sposob)
                 {
                    case 1:
                    {
                        cout << "Было выбрано: 1) двумерный массив фиксированного размера." << endl;
                        int Matrix[size][size];
                        sposob_zapolneniya(Matrix, M, N);
                        display(Matrix, M, N);
                        cout << endl;
                        ex1(Matrix, M, N);
                        break;
                    }
                    case 2:
                    {
                        cout << "Было выбрано: 2) матрица реализована с использованием библиотеки STL (класс array)." << endl;
                        array<array<int, size>, size> Matrix;
                        sposob_zapolneniya(Matrix, M, N);
                        display(Matrix, M, N);
                        cout << endl;
                        ex1(Matrix, M, N);
                        break;
                    }
                 }
                break;
            }
            case 2:
            {
                 cout << "Выполнение задания 2." << endl;
                 switch (sposob)
                 {
                    case 1:
                    {
                        cout << "Было выбрано: 1) двумерный массив фиксированного размера." << endl;
                        int Matrix[size][size];
                        sposob_zapolneniya(Matrix, M, N);
                        display(Matrix, M, N);
                        int i_row = ex2(Matrix, M,N);
                        if (i_row != -1) cout << "Номер строки: " << ex2(Matrix, M, N) << endl;
                        else cout << "В матрице не было найдено такой строки." << endl;
                        break;
                    }
                    case 2:
                    {
                        cout << "Было выбрано: 2) матрица реализована с использованием библиотеки STL (класс array)." << endl;
                        array<array<int, size>, size> Matrix;
                        sposob_zapolneniya(Matrix, M, N);
                        display(Matrix, M, N);
                        int i_row = ex2(Matrix, M,N);
                        if (i_row != -1) cout << "Номер строки: " << ex2(Matrix, M, N) << endl;
                        else cout << "В матрице не было найдено такой строки." << endl;
                        break;
                    }
                 }
                 break;
            }
            case 3:
            {
                 cout << "Выполнение задания 3." << endl;
                 switch (sposob)
                 {
                    case 1:
                    {
                        cout << "Было выбрано: 1) матрица реализована с использованием библиотеки STL (класс array)." << endl;
                        array<array<int, size>, size> Matrix;
                        sposob_zapolneniya(Matrix, M, N);
                        display(Matrix, M, N);
                        ex3(Matrix, M, N);
                        cout << "Преобразованная матрица: " << endl;
                        display(Matrix, M, N);
                        break;
                    }
                    case 2:
                    {
                        cout << "Было выбрано: 2) динамический двумерный массив (массив указателей на строки)." << endl;
                        int **Matrix = new int *[M];

                        for(int i = 0; i < M; i++)
                        {
                            Matrix[i] = new int[N];
                        }

                        sposob_zapolneniya(Matrix, M, N);
                        display(Matrix, M, N);
                        ex3(Matrix, M, N);
                        cout << "Преобразованная матрица: " << endl;
                        display(Matrix, M, N);
                        break;
                    }
                    case 3:
                    {
                        cout << "Было выбрано: 3) матрица реализована с использованием библиотеки STL (класс vector)." << endl;
                        vector<vector<int>> Matrix;
                        sposob_zapolneniya(Matrix, M, N);
                        display(Matrix, M, N);
                        ex3(Matrix, M, N);
                        cout << "Преобразованная матрица: " << endl;
                        display(Matrix, M, N);
                        break;
                    }
                 }
                 break;
            }
            case 4:
            {
                 cout << "Выполнение задания 4.";
                 switch (sposob)
                 {
                    case 1:
                    {
                        cout << "Было выбрано: 1) динамический двумерный массив (массив указателей на строки)." << endl;

                        int **Matrix = new int *[M];

                        for(int i = 0; i < M; i++)
                        {
                            Matrix[i] = new int[N];
                        }
                        sposob_zapolneniya(Matrix, M, N);
                        display(Matrix, M, N);

                        int M_new = M+1;
                        int N_new = N+1;

                        int **Matrix_new = new int *[M_new];

                        for(int i = 0; i < M_new; i++)
                        {
                            Matrix_new[i] = new int[N_new];
                        }

                        ex4(Matrix, Matrix_new, M, N);

                        for (int i = 0; i < M; i++) delete[] Matrix[i];
                        delete[] Matrix;

                        cout << endl;
                        display(Matrix_new, M_new, N_new);

                        for (int i = 0; i < M; i++) delete[] Matrix_new[i];
                        delete[] Matrix_new;

                        break;
                    }
                    case 2:
                    {
                        cout << "Было выбрано: 2) матрица реализована с использованием библиотеки STL (класс vector)." << endl;
                        vector<vector<int>> Matrix;
                        sposob_zapolneniya(Matrix, M, N);
                        display(Matrix, M, N);
                        ex4(Matrix,M,N);
                        cout << "Преобразованная матрица: " << endl;
                        display(Matrix, M, N);
                        break;
                    }
                 }
                 break;
            }
            case 5:
            {
                 cout << "Выполнение задания 5." << endl;
                 switch (sposob)
                 {
                    case 1:
                    {
                        cout << "Было выбрано: 1) динамический двумерный массив (массив указателей на строки)." << endl;
                        int **Matrix = new int *[M];

                        for(int i = 0; i < M; i++)
                        {
                            Matrix[i] = new int[N];
                        }
                        sposob_zapolneniya(Matrix, M, N);
                        display(Matrix, M, N);

                        int M_new = M;
                        new_size_M(Matrix, M_new, N);
                        if (M_new != M)
                        {
                            int **Matrix_temp = new int *[M_new];
                            for(int i = 0; i < M_new; i++)
                            {
                                Matrix_temp[i] = new int[N];
                            }
                            ex5_1(Matrix, Matrix_temp, M, N);
                            cout << "Преобразованная матрица (после удаления строк): " << endl;
                            display(Matrix_temp, M_new, N);

                            int chislo;
                            cout << "Введите заданное число: ";
                            cin >> chislo;

                            for (int i = 0; i < M; i++) delete[] Matrix[i];
                            delete[] Matrix;


                            int N_new = N;
                            new_size_N(Matrix_temp, M_new, N_new, chislo);
                            if (N_new != N)
                            {
                                int **Matrix_new = new int *[M_new];

                                for(int i = 0; i < M_new; i++)
                                {
                                    Matrix_new[i] = new int[N_new];
                                }

                                ex5_2(Matrix_temp, Matrix_new, M_new, N, chislo);

                                cout << "Преобразованная матрица (после удаления строк и столбцов): " << endl;
                                display(Matrix_new, M_new, N_new);

                                for (int i = 0; i < M_new; i++) delete[] Matrix_new[i];
                                delete[] Matrix_new;
                            }
                        } else {
                            cout << "Таких строк в матрице не нашлось." << endl;

                            int chislo;
                            cout << "Введите заданное число: ";
                            cin >> chislo;

                            int N_new = N;
                            new_size_N(Matrix, M, N_new, chislo);
                            if (N_new != N)
                            {
                                int **Matrix_new = new int *[M];

                                for(int i = 0; i < M; i++)
                                {
                                    Matrix_new[i] = new int[N_new];
                                }

                                ex5_2(Matrix, Matrix_new, M, N, chislo);

                                cout << "Преобразованная матрица (после удаления столбцов): " << endl;
                                display(Matrix_new, M, N_new);

                                for (int i = 0; i < M; i++) delete[] Matrix_new[i];
                                delete[] Matrix_new;
                            } else cout << "Таких столбцов не нашлось." << endl;
                        }

                        break;
                    }
                    case 2:
                    {
                        cout << "Было выбрано: 2) матрица реализована с использованием библиотеки STL (класс vector)." << endl;
                        vector<vector<int>> Matrix;
                        sposob_zapolneniya(Matrix, M, N);
                        display(Matrix, M, N);
                        ex5(Matrix,M,N);
                        cout << "Преобразованная матрица: " << endl;
                        display(Matrix, M, N);
                        break;
                    }
                 }
                 break;
            }
            }
        }
    }
    cout << "Программа была завершена по желанию пользователя.";
    return 0;
}
