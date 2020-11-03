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

string Graph_AM::toString() const noexcept
{
    stringstream ss;
    // шапка матрицы (номера столбцов)
    ss << setw(3) << "\\" << setw(4) << "E" << setw(2) << "|" << endl;
    ss << setw(4) << "\\" << setw(5) << "|" << endl;
    ss << setw(2) << "V" << setw (3) << "\\" << setw(4) << "|";
    for (size_type k = 0; k < V; ++k) ss << "\t" << k+1;
    const string border_str(10 * V,'-');
    ss << endl << border_str << endl;
    // сама матрица и номера строк
    for (size_type i = 0; i < V; ++i)
    {
        ss << i+1 << "\t" << "|";
        for (size_type j = 0; j < V; ++j)
        {
            ss << "\t";
            ss << int(M[i][j]);
        }
        ss << endl << border_str << endl;
    }
    return ss.str();
}

ostream& operator<<(ostream &t, const Graph_AM &r)
{
    string s;
    s = r.toString();
    return (t << s);
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
