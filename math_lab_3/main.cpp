//ver 2 - 22.04.19


#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <windows.h>


using namespace std;

struct Points
{
    short int x, y, i, j;
};

typedef vector<bool> row;
typedef vector<row> matrix;

//генерация вектора
void generation_vector (vector<int> &M, const int &a, const int &b)
{
    // шаблон int k = a + rand() % ( b - a + 1 );
    int ostatok = (b-a)+1;
    for (vector<int>::iterator i = M.begin(); i != M.end(); ++i)
    {
         *i = a + rand() % ostatok; // присваеваем элементу под индексом i сгенерированное случайное число
    }
}

//генерация множеств X и Y
void generation_vector_ex2 (const vector<int> &A, vector<int> &X, const int &n, const int &m)
{
    int j = 0;
    for (int i = n-1; i > 0 && j < m; i--)
    {
         X[j] = A[i];
         j++;
    }
}

//вывод массива
void print_array (const vector<int> &M)
{
    for (vector<int>::const_iterator i = M.begin(); i != M.end(); ++i) cout << "\t" << *i;
    cout << endl;
}

// декартово произведение X и Y
void print_dec (const vector<int> &X, const vector<int> &Y)
{
    cout << "Декартово проивзедение X x Y: \n { ";
    for (vector<int>::const_iterator i = X.begin(); i != X.end(); ++i) {
        for (vector<int>::const_iterator j = Y.begin(); j != Y.end(); ++j) {
            cout << "(" << *i << "; " << *j << ")";
        }
    }
    cout << " }" << endl;
}

// проверка на повторяющееся индексы точки, k - size of P, возвращает ложь если повторяется
bool checknapovtor (const vector<Points> &P, const int &k, const int &k_x, const int &k_y)
{
    for (int i = 0; i < k; i++)
    {
        if( (P[i].i == k_x) || (P[i].j == k_y) )
        {
            return false;
        }
    }
    return true;
}

// Поиск точек для отношения (составление пар неповторяющихся по индексу)
void find_points (vector<Points> &P, const vector<int> &A, const vector<int> &B, const int &n)
{
    int k = 0;
    int k_x;
    int k_y;
    for (int i = 0; i < n; i++)
    {
        do {
            k_x = rand() % n;
            k_y = rand() % n;
        } while (!checknapovtor(P, k, k_x, k_y));
        P[i].x = A[k_x];
        P[i].y = B[k_y];
        P[i].i = k_x;
        P[i].j = k_y;
        k++;
    }
}

// Составленные упорядоченные неповторяющиееся по индексу пары (из которых потом строится отношение)
void print_points(const vector<Points> &P)
{
    cout << "Случайный, неповторяющийся по индексу набор точек: \n { ";
    for (vector<Points>::const_iterator i = P.begin(); i != P.end(); ++i) {
            cout << "(" << i -> x << "; " << i -> y << ")";
    }
    cout << " }" << endl;
}

// Вывод координат точек в файл
void output_points_in_file(const vector<Points> &P)
{
    ofstream out("dots.txt");
    if (out) // проверка открытия
    {
        for (vector<Points>::const_iterator i = P.begin(); i != P.end(); ++i)
        {
            out << i -> x << " " << i -> y << " ";
        }
        out.close();
    }
}

// Создание нулевой матрицы для отношения
void create_empty_matrix (matrix &BOOL, const int &n)
{
    row Row(n,0);
    for (int i = 0; i < n; i++)
    {
        BOOL.push_back(Row);
    }
}

// Условие отношения
bool uslovie_ex (const int &i, const int &j, const vector<Points> &P) // условие отношения
{
    for (vector<Points>::const_iterator k = P.begin(); k != P.end(); ++k) {
            if ( (k -> i == i) && (k -> j == j) ) return 1;
    }
    return 0;
}

// Формирование отношения
void ex (matrix &BOOL, const vector<Points> &P, const int &n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            BOOL[i][j] = uslovie_ex(i, j, P);
        }
    }
}

// Вывод отношения
void print_matrix (const matrix &BOOL, const vector<int> &A, const int &n)
{
    for (int i = 0; i < n; i++)
    {
        cout << A[i] << "\t" << "|";
        for(int j = 0; j < n; j++)
        {
            cout << "\t";
            cout << BOOL[i][j];
        }
        cout << endl;
        cout << setfill('-') << setw(60);
        cout << "\n";
        cout << setfill(' ');
    }
}

bool proverka_na_func (const vector<Points> &P, const int &n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i+1; j < n; j++)
        {
            if (P[i].x == P[j].x)
            {
                if (P[i].y != P[j].y) return false;
            }
        }
    }
    return true;
}

bool proverka_na_inject (const vector<Points> &P, const int &n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i+1; j < n; j++)
        {
            if (P[i].y == P[j].y)
            {
                if (P[i].x != P[j].x) return false;
            }
        }
    }
    return true;
}

bool proverka_na_mono (const vector<Points> &P, const int &n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i+1; j < n; j++)
        {
            if ( ((P[i].x < P[j].x) && (P[i].y > P[j].y)) || ((P[i].x > P[j].x) && (P[i].y < P[j].y)) )
            {
                return false;
            }
        }
    }
    return true;
}



int main()
{
    srand((unsigned)time(0));
    system("chcp 1251 > nul");
    cout << "Измайлов Наиль и Катунин Сергей, ДИПРБ-11. \n Дискретная математика, Лабораторная работа №3." << endl;
    int n, a, b, m;
    cout << "Введите число n (размер множеств): ";
    cin >> n;
    while (n < 1)
    {
        cout << "error!" << endl;
        cout << "Введите число n (размер множеств): ";
        cin >> n;
    }
    cout << "Введите число a (нижняя граница случайных чисел): ";
    cin >> a;
    cout << "Введите число b (верхняя граница случайных чисел): ";
    cin >> b;
    while (b <= a)
    {
        cout << "error!" << endl;
        cout << "Введите число b (верхняя граница случайных чисел): ";
        cin >> b;
    }
    vector<int> A(n);
    vector<int> B(n);
    int ch = 1;
    while (ch == 1)
    {
        generation_vector(A,a,b);
        generation_vector(B,a,b);
        sort( A.begin(), A.end() ); // сортировка
        sort( B.begin(), B.end() );
        cout << "Полученное множество A: ";
        print_array(A);
        cout << "Полученное множество B: ";
        print_array(B);
        cout << "Введите число m (количество последних элементов множеств A и B): ";
        cin >> m;
        while (m < 1 || m >= n)
        {
            cout << "error!" << endl;
            cout << "Введите число m (количество последних элементов множеств A и B): ";
            cin >> m;
        }
        vector<int> X(m);
        vector<int> Y(m);
        generation_vector_ex2(A,X,n,m);
        generation_vector_ex2(B,Y,n,m);
        sort( X.begin(), X.end() ); // сортировка
        sort( Y.begin(), Y.end() );
        print_array(X);
        print_array(Y);
        print_dec(X, Y);
        vector<Points> P(n);
        find_points(P, A, B, n);
        print_points(P);
        output_points_in_file(P);

        matrix BOOL; // объявление матрицы
        create_empty_matrix(BOOL, n); // создание пустой матрицы
        ex(BOOL, P, n); // заполнение матрицы в соответствие с заданием 1

        // Оформление шапки матрицы
        cout << setw(3) << "\\" << setw(4) << "B" << setw(2) << "|" << endl;
        cout << setw(4) << "\\" << setw(5) << "|" << endl;
        cout << setw(2) << "A" << setw (3) << "\\" << setw(4) << "|";
        print_array(B);
        cout << setfill('-') << setw(60);
        cout << "\n";
        cout << setfill(' ');
        print_matrix(BOOL, A, n); // Вывод матрицы
        cout << "Является ли отношение R функцией: ";
        if (proverka_na_func(P,n))
        {
            cout << "Да." << endl;
            cout << "Является ли функция R биективной: " << proverka_na_inject(P,n) << endl;
            cout << "Является ли функция R монотонной: " << proverka_na_mono(P,n) << endl;
        } else cout << "Нет." << endl;

        system("graphics\\graph.exe");


        cout << "1. Продолжить" << endl << "2. Выйти" << endl;
        cout << "Ввод: ";

        cin >> ch;
        while (ch < 1 || ch > 2)
        {
            cout << "error!" << endl;
            cout << "Повторите ввод." << endl;
            cin >> ch;
        }
    }
    return 0;
}
