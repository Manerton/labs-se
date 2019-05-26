#include <iostream>
#include <cmath>
#include <time.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <fstream>

using namespace std;


typedef vector<int> row;
typedef vector<row> graf;

void clean (graf &A, const int &n)
{
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            A[i][j] = 0;
        }
    }
}
void create_empty_matrix (graf &A, const int &n)
{
    row Row(n,0);
    for (int i=0; i<n; i++)
    {
        A.push_back(Row);
    }
}

void hand_init_rebro_input(int &ai, int &bi, const int &n)
{
    cout << " Введите номер первого элемента: ";
    cin >> ai;
    while ((ai < 1) || (ai > n))
    {
        cout << " Ошибка! " << endl;
        cout << " Введите номер первого элемента: ";
        cin >> ai;
    }
    cout << " Введите номер второго элемента: ";
    cin >> bi;
    while ((bi < 1) || (bi > n) || (bi == ai))
    {
        cout << " Ошибка! " << endl;
        cout << " Введите номер второго элемента: ";
        cin >> bi;
    }
    ai--; // так как ввели номера, а не индексы
    bi--;
}

void hand_init_rebro (graf &A, const int &n, const int &m)
{
    int ai;
    int bi;
    for (int j=0; j<m; j++)
    {
        hand_init_rebro_input(ai,bi,n);
        while (A[ai][bi] == 1)
        {
            cout << " Ошибка! Вы ввели кратное ребро! Повторите ввод!" << endl;
            hand_init_rebro_input(ai,bi,n);
        }
        A[ai][bi] = 1;
    }
}
void random_init_rebro (graf &A, const int &n, const int &m)
{
    int ai;
    int bi;
    for (int j=0; j<m; j++)
    {
        do {
            ai = rand()%n;
            do {
                bi = rand()%n;
            } while (bi == ai);
        } while (A[ai][bi] == 1);
        A[ai][bi] = 1;
    }
}

graf create_podgraf (const graf &A, const int &k)
{
    graf p(A.end()-k, A.end());
    int N = p.size();
    for (int i = 0; i < N; i++)
    {
        p[i].erase(p[i].begin(), p[i].end()-k);
    }
    return p;
}

void print_graf (const graf &A)
{
    int n = A.size();
    cout << setw(3) << "\\" << setw(4) << "A" << setw(2) << "|" << endl;
    cout << setw(4) << "\\" << setw(5) << "|" << endl;
    cout << setw(2) << "A" << setw (3) << "\\" << setw(4) << "|";
    for (int j=0; j<n; j++)
    {
        cout << "\t" << j+1;

    }
    cout << endl;
    cout << setfill('-') << setw(60);
    cout << "\n";
    cout << setfill(' ');
    for (int i=0; i<n; i++)
    {
        cout << i+1 << "\t" << "|";
        for(int j=0; j<n; j++)
        {
            cout << "\t" << A[i][j];
        }
        cout << endl;
        cout << setfill('-') << setw(60);
        cout << "\n";
        cout << setfill(' ');

    }
}

void diagramm_way (const graf &A)
{
    int n = A.size();
    for (int i=0; i<n; i++)
    {
        int c = 0;
        for (int j=0; j<n; j++)
        {
            if (A[i][j] == 1)
            {
                if (c == 0) { cout << i+1 << " -> "; }
                cout << "\t" << j+1;
                c++;
            }
        }
        if (c != 0) { cout << endl; }
    }
}

void R_R (const graf &A, const int &n, graf &R)
{
    R = A;
    for (int k=0; k<n; k++)
    {
        for (int i=0; i<n; i++)
        {
            for (int j=0; j<n; j++)
            {
                if (R[i][j]==0)
                {
                    if ((R[i][k]==1) && (R[k][j]==1)) { R[i][j]=1; }
                }
            }
        }
    }
}
bool strong_way (const graf &R, const int &n)
{
    for (int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            if (R[i][j] == 0) { return false; }
        }
    }
    return true;
}

void Q_Q (const graf &R, const int &n, graf &Q)
{
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            Q[i][j] = R[j][i];
        }
    }
}

void S_S_S (const graf &A, const int &n, graf &S_e)
{
    S_e = A; // S в 1 степени, т.е матрица смежности
    graf S; // темповый S
    S = A;
    graf S_i;
    create_empty_matrix(S_i, n);

    for (int s=2; s<n+1; s++)
    {
        for (int i=0; i<n; i++)
        {
            for (int j=0; j<n; j++)
            {
                for (int k=0; k<n; k++)
                {
                    S_i[i][j] += (S[i][k] * A[k][j]);
                }

            }
        }
        cout << "S^" << s << ": " << endl;
        print_graf(S_i);
        S = S_i;
        system("pause");
        for (int i=0; i<n; i++)
        {
            for (int j=0; j<n; j++)
            {
                S_e[i][j] += S[i][j];

            }
        }
        clean(S_i, n);
    }
}

void more_way (const graf &S_e, const int &n, int &i_m, int &j_m)
{
    int ways = 0;
    for (int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            if (S_e[i][j] > ways){
                ways = S_e[i][j];
                i_m = i;
                j_m = j;
            }
        }
    }
}

void find_cycles (const graf &S_e, const int &n)
{
    int j = 0;
    for (int i=0; i<n; i++)
    {
        if (S_e[i][i] != 0)
        {
            cout << " • Цикл с вершиной " << i+1 << " существует." << endl;
            j++;
        }
    }
    if (j == 0) cout << "Циклов в графе не существует." << endl;
}

/*void dfs (const int &v, const vector<vector<int>> &g, int &cycle_end, int &cycle_st, vector<int> &used, vector<int> &p)
{
    used[v] = 1;
    for (size_t j=0; j<g[v].size(); j++) {
        int to = g[v][j];
        if (used[to] == 0) {
            p[to] = v;
            dfs (to, g, cycle_end, cycle_st, used, p);
        } else if (used[to] == 1) {
            cycle_end = v; //Текущая вершина – конец цикла
            cycle_st = to; //Вершина, в которую пытаемся перейти - начало цикла
            int k = cycle_end;
            cout << cycle_st+1 << " ";
            while (k != cycle_st) {
                cout << k+1 << " ";
                k = p[k];
            }
            cout << cycle_st+1 << endl;
        }
    }
    used[v] = 0; // если поставить 0, то будут повторы, если поставить 2, то выведет не все циклы
}
void main_zikl (const graf &A, const int &n)
{
    int cycle_st (0), cycle_end (0);
    vector<vector<int>> g(n);
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++){
            if(A[i][j] == 1) { g[i].push_back(j); }
        }
    }
    vector<int> used(n);
    for (int i=0; i<n; i++) {
        used.push_back(0);
    }
    vector<int> p;
    for (int i=0; i<n*n; i++) {
        p.push_back(0);
    }
    for (int i=0; i<n; i++){
        if (used[i] == 0) {
            dfs(i, g, cycle_end, cycle_st, used, p);
        }
    }
}*/


int main()
{
    srand(time(0));
    system("chcp 1251 > nul");
    cout << "Измайлов Наиль и Катунин Сергей, ДИПРБ-11. \n Дискретная математика, Лабораторная работа №4." << endl;
    int ch = 1;
    while (ch == 1)
    {
        int n; //количество вершин
        int m; //количество ребер
        int k = 0; //количество вершин подграфа
        do {
            cout << " Введите количество вершин [3, 6]: ";
            cin >> n;
        } while ((n < 3) || (n > 6));
        do {
            cout << " Введите количество ребер [2, " << 2*n << "]: ";
            cin >> m;
        } while ((m < 2) || (m > 2*n));

        graf A;
        create_empty_matrix(A, n);

        int init;
        int ok = 0;
        while (ok != 2) {
            clean(A,n);
            cout << " Ввод ребер \n 1. С клавиатуры \n 2. Случайно" << endl;
            do {
                cout << " \nВвод: ";
                cin >> init;
            } while ((init < 1) || (init > 2));
            if (init == 1) {
                hand_init_rebro(A, n, m);
            } else {
                random_init_rebro(A, n, m);
            }
            cout << " Матрица смежности: " << endl;
            print_graf(A);
            cout << " Пересгенерировать матрицу? \n  1. Да \n  2. Нет" << endl;
            do {
                cout << " \nВвод: ";
                cin >> ok;
            } while ((ok < 1) || (ok > 2));
        }

        cout << " Диаграмма: " << endl;
        diagramm_way(A);

        do {
            cout << " Введите количество вершин подграфа [1, " << n-1 << "]: ";
            cin >> k;
        } while ((k < 1) || (k > n-1));
        cout << " Матрица смежности подграфа: " << endl;
        graf A_podgraf = create_podgraf(A,k);
        print_graf(A_podgraf);
        cout << " Диаграмма: " << endl;
        diagramm_way(A_podgraf);
        system("pause");

        graf R;
        create_empty_matrix(R, n);
        R_R(A, n, R);
        cout << "R: " << endl;
        print_graf(R);
        system("pause");

        graf Q;
        create_empty_matrix(Q, n);
        Q_Q(R, n, Q);
        cout << "Q: " << endl;
        print_graf(Q);
        system("pause");

        cout << " Является ли граф сильно связанным? ";
        if(strong_way(R, n)) {
            cout << " Да.";
        } else { cout << " Нет."; }
        cout << endl;
        system("pause");

        graf S_e;
        create_empty_matrix(S_e, n);
        S_S_S(A, n, S_e);
        cout << "S^e: " << endl;
        print_graf(S_e);
        system("pause");

        int i_m (0), j_m (0);
        more_way(S_e, n, i_m, j_m);
        cout << " Больше всего путей между "  << i_m+1 << " и " << j_m+1 << endl;
        system("pause");

        cout << " Поиск циклов в графе: " << endl;
        find_cycles(S_e,n);

        //main_zikl(A, n);
        system("pause");

        cout << " \n 0. Выход \n 1. Пересгенерировать" << endl;
        do {
            cout << " \nВвод: ";
            cin >> ch;
        } while ((ch < 0) || (ch > 1));
    }
    return 0;
}
