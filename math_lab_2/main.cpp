//ver 3 - 08.04.19 // bug с композицией


#include <iostream>
#include <cmath>
#include <time.h>
#include <vector>
#include <algorithm>


using namespace std;

typedef vector<bool> row;
typedef vector<row> matrix;


//проверка на повторение элемента, M - вектор, k - число на проверку
bool checknapovtor (vector<int> M, int k)
{
    for (vector<int>::iterator i = M.begin(); i != M.end(); ++i)
    {
         if(*i == k) return false;
    }

    return true;
}

//генерация вектора
void generation_vector (vector<int> &M)
{
    // шаблон int k = a + rand() % ( b - a + 1 );
    int k; // диапазон [-10;10]
    for (vector<int>::iterator i = M.begin(); i != M.end(); ++i)
    {
        do {
            k = -10 + rand() % ( 21 );
        } while (!checknapovtor(M, k));
         *i = k; // присваеваем элементу под индексом i значение k
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
   // cout << '\t';
    //for (vector<int>::const_iterator i = M.begin(); i != M.end(); ++i) cout << *i << '\t';
    //cout << endl;
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


bool uslovie_ex1 (int x, int y) // условие отношения
{
    if ( (x % 2 == 0) && (x <= y) ) return 1;
    else return 0;
}

void ex1 (matrix &A, const vector<int> &M) // первое задание (матрица A 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - четное}
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            A[i][j] = uslovie_ex1(M[i], M[j]);
        }
    }
}

void ex2 (matrix &B, const matrix &A) // второе задание (матрица B 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - четное}
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            B[i][j] = 1 - A[i][j];
        }
    }
}

void ex3 (matrix &C, const vector<int> &M) // третье задание (матрица C 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - четное}
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            C[i][j] = uslovie_ex1(M[j], M[i]);
        }
    }
}

void ex4 (matrix &D, const matrix &A, const matrix &B) // четвертое задание (матрица D 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - четное}
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                D[i][j] =+ (A[i][k] && B[k][j]);
            }
        }
    }
}

void ex5 (matrix &E, const matrix &D) // пятое задание (матрица E 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - четное}
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            E[j][i] = 1 - D[i][j];
        }
    }
}

void ex6 (matrix &F, const matrix &A, const matrix &D) // шестое задание (матрица F 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - четное}
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            F[i][j] = (A[i][j] && D[i][j]);
        }
    }
}

bool ex7_a (const matrix &E) // Свойство рефлексивность (матрица E 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - четное}
    for (int i = 0; i < 5; i++)
    {
        if (E[i][i] != 1) return false;
    }
    return true;
}

bool ex7_b (const matrix &E) // Свойство антирефлексивность (матрица E 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - четное}
    for (int i = 0; i < 5; i++)
    {
        if (E[i][i] != 0) return false;
    }
    return true;
}

bool ex7_g (const matrix &E) // Свойство симметричность (матрица E 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - четное}
    for (int i = 0; i < 5; i++)
    {
        for (int j = (i+1); j < 5; j++)
        {
            if (E[i][j] != E[j][i]) return false;
        }
    }
    return true;
}

bool ex7_d (const matrix &E) // Свойство антисимметричность (матрица E 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - четное}
    for (int i = 0; i < 5; i++)
    {
        for (int j = (i+1); j < 5; j++)
        {
            if (E[i][j] == 1 && E[i][j] == E[j][i]) return false;
        }
    }
    return true;
}

bool ex7_e (const matrix &E) // Свойство асимметричность (матрица E 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - четное}
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (E[i][j] == E[j][i]) return false;
        }
    }
    return true;
}

bool ex7_z (const matrix &E) // Свойство транзитивность (матрица E 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - четное}
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                if ((E[i][k] == E[k][j]) && (E[i][k] == 1) && (E[i][j] == 0)) return false;
            }
        }
    }
    return true;
}

int main()
{
     // R = { (x,y) E A^2 | x <= y, x - четное}
    srand((unsigned)time(0));
    system("chcp 1251 > nul");
    cout << "Измайлов Наиль и Катунин Сергей, ДИПРБ-11. \n Дискретная математика, Лабораторная работа №2." << endl;
    vector<int> M(5);
    int ch = 1;
    while (ch == 1)
    {
        generation_vector(M);
        sort( M.begin(), M.end() ); // сортировка
        cout << "Полученное множество M: ";
        print_array(M);
        cout << "\nR = { (x,y) E A^2 | x <= y, x - четное }." << endl;
        cout << "\n1. Построить матрицу А отношения R." << endl;
        matrix A; // объявление матрицы
        create_empty_matrix(A); // создание пустой матрицы
        ex1(A, M); // заполнение матрицы в соответствие с заданием 1
        print_matrix(A); // Вывод матрицы

        cout << "\n2. Построить матрицу B дополнения R` отношения R." << endl;
        matrix B; // объявление матрицы
        create_empty_matrix(B); // создание пустой матрицы
        ex2(B, A); // заполнение матрицы в соответствие с заданием 2
        print_matrix(B); // Вывод матрицы

        cout << "\n3. Построить матрицу C обращения R^-1 отношения R." << endl;
        matrix C; // объявление матрицы
        create_empty_matrix(C); // создание пустой матрицы
        ex3(C, M); // заполнение матрицы в соответствие с заданием 3
        print_matrix(C); // Вывод матрицы

        cout << "\n4. Вычислить матрицу D произведения отношений Rп = R * R`." << endl;
        matrix D; // объявление матрицы
        create_empty_matrix(D); // создание пустой матрицы
        ex4(D, A, B); // заполнение матрицы в соответствие с заданием 3
        print_matrix(D); // Вывод матрицы

        cout << "\n5. Вычислить матрицу E двойственного отношения (Rп)^d к отношению Rп." << endl;
        matrix E; // объявление матрицы
        create_empty_matrix(E); // создание пустой матрицы
        ex5(E, D); // заполнение матрицы в соответствие с заданием 3
        print_matrix(E); // Вывод матрицы

        cout << "\n6. Вычислить матрицу F отношения R (пересечение) Rп." << endl;
        matrix F; // объявление матрицы
        create_empty_matrix(F); // создание пустой матрицы
        ex6(F, A, D); // заполнение матрицы в соответствие с заданием 3
        print_matrix(F); // Вывод матрицы

        cout << "\n7. Проверить свойства (Rп)^d:" << endl;
        cout << "(1 - да, 0 - нет)" << endl;
        cout << "а) рефлексивность - " << ex7_a(E) << endl; // в главной диагонали только единицы
        cout << "б) антирефлексивность - " << ex7_b(E) << endl; // в главной диагонали только нули
        cout << "в) иррефлексивность - "; // в главной диагонали есть хотя бы одна единица
            if (ex7_b(E)) cout << false << endl;
            else cout << true << endl;
        cout << "г) симметричность - " << ex7_g(E) << endl; // симметрия относ главной диагонали
        cout << "д) антисимметричность - " << ex7_d(E) << endl; // относительно главной диагонали нет симметричных единиц
        cout << "е) асимметричность - " << ex7_e(E) << endl; // симметричных элементов относительно диагонали нет
        cout << "ж) транзитивность - " << ex7_z(E) << endl; // квадрат матрицы равен матрице исходной, или условие в ex7_z
        cout << "\nМЕНЮ:" << endl;
        cout << "1. Пересгенерировать множество M." << endl;
        cout << "2. Завершить работу программы." << endl;
        cin >> ch;
        while (ch < 1 || ch > 2)
        {
            cout << "error!" << endl;
            cout << "Повторите ввод." << endl;
            cin >> ch;
        }
        // для теста свойств
        /*if (!ex7_a(E)) ch = 1;
        else cin >> ch;*/
    }
    return 0;
}
