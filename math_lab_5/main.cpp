#include <iostream>
#include <cmath>
#include <ctime>
#include <limits.h>
#include <vector>
#include <iomanip>
#include <fstream>
#include <windows.h>

using namespace std;

typedef vector<int> row;
typedef vector<row> graf;

void clean (graf &A)
{
    for (int i=0; i<5; i++)
    {
        for (int j=0; j<5; j++)
        {
            A[i][j] = 0;
        }
    }
}
void create_empty_matrix (graf &A)
{
    row Row(5,0);
    for (int i=0; i<5; i++)
    {
        A.push_back(Row);
    }
}

void hand_init_weight (graf &A, bool &alg)
{
    int n = A.size();
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            if (j != i) {
                cout << " Вес связи " << i+1 << "-" << j+1 << " [-10; 10]: ";
                cin >> A[i][j];
                while ((A[i][j] < -10) || (A[i][j] > 10))
                {
                    cout << " Ошибка! Вы зашли за границы! Повторите ввод: ";
                    cin >> A[i][j];
                }
                if (A[i][j] < 0) { alg = false; }
            }
        }
    }
}
void random_init_weight (graf &A, bool &alg)
{
    int n = A.size();
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            if (j != i) {
                //A[i][j] = -10 + rand()%22;
                A[i][j] = rand()%11;
                if (A[i][j] < 0) { alg = false; }
            }
        }
    }
}

void diagramm_way (const graf &A)
{
    ofstream out("graph.dot");
    out << "digraph {\n";
    int n = A.size();
    for (int i=0; i<n; i++)
    {
        int c = 0;
        for (int j=0; j<n; j++)
        {
            if (A[i][j] != 0)
            {
                out << i+1 << "->" << j+1 << "[label=\"" << A[i][j] << "\"];" << endl;
            } else c++;
        }
        if (c == n) out << i+1 << endl;
    }
    out << "}\n";
    out.close();
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
            cout << "\t";
            if (i != j && A[i][j] == 0) {
               cout << "inf";
            } else { cout << A[i][j]; }
        }
        cout << endl;
        cout << setfill('-') << setw(60);
        cout << "\n";
        cout << setfill(' ');

    }
}

void Deikstra(const graf &A, const int &st)
{
    cout << "Промежуточные результаты работы алгоритма Дейкстры: " << endl;
    cout << " i\td[1]\td[2]\td[3]\td[4]\td[5]" << endl; // шапка промежуточных выводов
    int N = A.size();
    vector<int> distance = A[st]; // вектор - длины путей у каждой вершины
    for (int i = 0; i < N; i++)
    {
        if ((distance[i] == 0) && (st != i)) distance[i] = INT_MAX;
    }
    cout << " 1\t";
    for (int i=0; i<N; i++) // вывожу первую строку по алгоритму
    {
        if (distance[i]!=INT_MAX) cout<<distance[i]<< "\t";
        else cout<< "inf"<< "\t";
    }
    cout << endl;
    vector<bool> visited(N, false); // вектор - проверена ли вершина
    visited[st] = true; // исключаю источник из множества вершин T
    for (int S=1; S< N-1; S++) // до N-1, так как последнюю вершину нет смысла рассматривать, так как все соседи уже проверены
    {
        int min=INT_MAX;
        int j(0);
        for (int i=0; i<N; i++)
        {
            if (!visited[i] && distance[i]<=min) // ищем ближайшую нерассмотренную вершину с минимальной длиной пути
            {
                min=distance[i];
                j=i;
            }
        }
        visited[j]=true; // когда нашли, помечаем ее как рассмотренную, так как сейчас будем ее проверять
        for (int k=0; k<N; k++)
        {
            // если вершина j не рассмотрена, между текущей и j существует ребро,
            // проверка дистанс на != INT_MAX на случай если источник - изолирован ото всех, тогда и нет смысла ничего проверять,
            // если новый дистанс оказался меньше предыдущего
            if (!visited[k] && A[j][k] && distance[j]!=INT_MAX && distance[j]+A[j][k]<distance[k])
                distance[k]=distance[j]+A[j][k]; // тогда получаем новую длину пути (дистанс)
        }
        cout << " "<< S+1 << "\t";
        for (int i=0; i<N; i++)
        {
            if (distance[i]!=INT_MAX) cout<<distance[i]<< "\t";
            else cout<< "inf"<< "\t";
        }
        cout << endl;

    }
    int m=st+1;
    cout<<"Стоимость пути из начальной вершины до остальных:\t\n";
    for (int i=0; i<N; i++)
        if (distance[i]!=INT_MAX)
            cout<<m<<" > "<<i+1<<" = "<<distance[i]<<endl;
        else if (st == i) cout<<m<<" > "<<i+1<<" = "<<"0"<<endl;
        else
            cout<<m<<" > "<<i+1<<" = "<<"маршрут недоступен"<<endl;
}


void Ford_Bellman (const graf &A, const int &st)
{
    cout << "Промежуточные результаты работы алгоритма Дейкстры: " << endl;
    cout << " i\td[1]\td[2]\td[3]\td[4]\td[5]" << endl; // шапка промежуточных выводов
    int N = A.size();
    vector<int> distance = A[st]; // вектор - длины путей у каждой вершины
    for (int i = 0; i < N; i++)
    {
        if ((distance[i] == 0) && (st != i)) distance[i] = INT_MAX;
    }
    cout << " 1\t";
    for (int i=0; i<N; i++) // вывожу первую строку по алгоритму
    {
        if (distance[i]!=INT_MAX) cout<<distance[i]<< "\t";
        else cout<< "inf"<< "\t";
    }
    cout << endl;
    for (int S=1; S< N-1; S++) // до N-1, так как последнюю вершину нет смысла рассматривать, так как все соседи уже проверены
    {
        vector<int> distance_old = distance;
        for (int j=0; j<N; j++)
        {
            int min = distance_old[j];
            for (int k = 0; k < N; k++)
            {
                // если вершина j не рассмотрена, между текущей и j существует ребро,
                // проверка дистанс на != INT_MAX на случай если источник - изолирован ото всех, тогда и нет смысла ничего проверять,
                // если новый дистанс оказался меньше предыдущего

                if (A[k][j] && distance_old[k]!=INT_MAX && distance_old[k]+A[k][j]<min)
                    min=distance_old[k]+A[k][j]; // тогда получаем новую длину пути (дистанс)
            }
            distance[j] = min;
        }
        cout << " "<< S+1 << "\t";
        for (int i=0; i<N; i++)
        {
            if (distance[i]!=INT_MAX) cout<<distance[i]<< "\t";
            else cout<< "inf"<< "\t";
        }
        cout << endl;
        if (distance == distance_old) S = N;
    }
    int m=st+1;
    cout<<"Стоимость пути из начальной вершины до остальных:\t\n";
    for (int i=0; i<N; i++)
        if (distance[i]!=INT_MAX)
            cout<<m<<" > "<<i+1<<" = "<<distance[i]<<endl;
        else if (st == i) cout<<m<<" > "<<i+1<<" = "<<"0"<<endl;
        else
            cout<<m<<" > "<<i+1<<" = "<<"маршрут недоступен"<<endl;
}

int main()
{
    srand(time(0));
    system("chcp 1251 > nul");

    int ch = 1;
    while (ch == 1)
    {
        bool alg = true;
        graf A;
        create_empty_matrix(A);

        int init;
        int ok = 0;
        while (ok != 2) {
            clean(A);
            cout << " Заполнения матрицы весов \n 1. С клавиатуры \n 2. Случайно" << endl;
            do {
                cout << " \nВвод: ";
                cin >> init;
            } while ((init < 1) || (init > 2));
            if (init == 1) {
                hand_init_weight(A, alg);
            } else {
                random_init_weight(A, alg);
            }
            cout << " Матрица весов: " << endl;
            print_graf(A);
            cout << " Пересгенерировать матрицу? \n  1. Да \n  2. Нет" << endl;
            do {
                cout << " \nВвод: ";
                cin >> ok;
            } while ((ok < 1) || (ok > 2));
        }

        cout << " Открываю диаграмму графа... " << endl;
        diagramm_way(A);
        system("graphics\\circo.exe -Tpng -O graph.dot");
        system("graph.dot.png");
        system("pause");


        int st;
        do {
            cout << " Выберите вершину графа [1, 5]: ";
            cin >> st;
        } while ((st < 1) || (st > 5));
        if (alg)
        {
            int met;
            cout << " 1. Алгоритм Дейкстры. \n 2. Алгоритм Форда-Беллмана." << endl;
            do {
                cout << " \nВвод: ";
                cin >> met;
            } while ((met < 1) || (met > 2));
            if (met == 1) {
                Deikstra(A, st-1);

            } else { Ford_Bellman(A, st-1); }

        } else {
            cout << " Доступен только алгоритм Форда-Беллмана, так как хотя бы одна дуга имеет отрицательный вес." << endl;
            Ford_Bellman(A, st-1);
        }


        cout << " \n 0. Выход \n 1. Пересгенерировать" << endl;
        do {
            cout << " \nВвод: ";
            cin >> ch;
        } while ((ch < 0) || (ch > 1));
    }
    return 0;
}
