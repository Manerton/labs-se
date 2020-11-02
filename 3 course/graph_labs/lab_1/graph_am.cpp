#include <iomanip>
#include "graph_am.h"

using namespace std;

// вставить ребро
void Graph_AM::insert(Graph::Edge e) noexcept
{
    if (current_E < E) // иначе некуда вставлять
    {
        const size_type v = e.first;  // левая вершина
        const size_type w = e.second; // правая вершина

        // проверяем, нет ли уже ребра между вершинами v и w
        if ( M[v][w] == 0 )
        {
            M[v][w] = M[w][v] = 1;
            ++current_E;
        }
    }
}

void Graph_AM::remove(Graph::Edge e) noexcept
{
    if (current_E > 0) // иначе нечего удалять
    {
        const size_type v = e.first;  // левая вершина
        const size_type w = e.second; // правая вершина
        // проверяем, существует ли удаляемое ребро
        if ( M[v][w] == 1 )
        {
            M[v][w] = M[w][v] = 0;
            --current_E;
        }
    }
}

void Graph_AM::display() const noexcept
{
    // шапка матрицы (номера столбцов)
    cout << setw(3) << "\\" << setw(4) << "E" << setw(2) << "|" << endl;
    cout << setw(4) << "\\" << setw(5) << "|" << endl;
    cout << setw(2) << "V" << setw (3) << "\\" << setw(4) << "|";
    for (size_type k = 0; k < V; ++k) cout << "\t" << k+1;
    const string border_str(60,'-');
    cout << endl << border_str << endl;
    // сама матрица и номера строк
    for (size_type i = 0; i < V; ++i)
    {
        cout << i+1 << "\t" << "|";
        for (size_type j = 0; j < V; ++j)
        {
            cout << "\t";
            cout << int(M[i][j]);
        }
        cout << endl << border_str << endl;
    }
}

Graph::size_type Graph_AM::Iterator::operator++()
{
    // проходим по всем столбцам в матрице смежности
    const size_type V = G.getV();
    for (++i; i < V; ++i)
    {
        // если нашли вторую вершину i, смежную вершине v, то возвращаем вершину i
        if (G.M[v][i] == 1) return i;
    }
    return NOT_FOUND; // если не нашли ничего, но в связном графе этого не должно происходить
}
