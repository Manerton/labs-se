//ver 1 


#include <iostream>
#include <cmath>
#include <time.h>
#include <vector>
#include <algorithm>


using namespace std;

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

//генерация вектора
void generation_vector_ex2 (const vector<int> &A, vector<int> &X, int n, int m)
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
    for (vector<int>::const_iterator i = M.begin(); i != M.end(); ++i) cout << *i << " ";
    cout << endl;
}

void create_empty_matrix (matrix &A)
{
    row Row(5,0);
    for (int i = 0; i < 5; i++)
    {
        A.push_back(Row);
    }
}
void print_matrix (const matrix &A) // параметр const vector<int> &M
{
    for (int i = 0; i < 5; i++)
    {
       // cout << M[i] << '\t';
        for(int j = 0; j < 5; j++)
        {
            cout << A[i][j] << '\t';
        }
        cout << endl;
    }
}

void print_dec (const vector<int> &X, const vector<int> &Y)
{
    cout << "{ ";
    for (vector<int>::const_iterator i = X.begin(); i != X.end(); ++i) {
        for (vector<int>::const_iterator j = Y.begin(); j != Y.end(); ++j) {
            cout << "(" << *i << "; " << *j << ")";
        }
    }
    cout << " }" << endl;
}

int main()
{
    srand((unsigned)time(0));
    system("chcp 1251 > nul");
    cout << "Измайлов Наиль и Катунин Сергей, ДИПРБ-11. \n Дискретная математика, Лабораторная работа №2." << endl;
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
       /*
        matrix A; // объявление матрицы
        create_empty_matrix(A); // создание пустой матрицы
        ex1(A, M); // заполнение матрицы в соответствие с заданием 1
        print_matrix(A); // Вывод матрицы*/
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
