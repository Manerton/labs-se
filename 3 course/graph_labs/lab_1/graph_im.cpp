#include "graph_im.h"
#include <iomanip>

using namespace std;

// вставка ребер в матрицу инцидентности V x E
// по индексу current_E, т.е вставка осуществляется до E
void Graph_IM::insert(Graph_IM::Edge e) noexcept
{
    if (current_E < E) // иначе некуда вставлять
    {
        const size_type v = e.first;  // левая вершина
        const size_type w = e.second; // правая вершина

        // проверяем, нет ли уже ребра по индексу current_E
        if ( !((M[v][current_E] == 1) && (M[w][current_E] == 1)) )
        {
            M[v][current_E] = M[w][current_E] = 1;
            ++current_E;
        }
    }
}

// удаление ребра из матрицы инцидентности V x E
void Graph_IM::remove(Graph_IM::Edge e) noexcept
{
    if (current_E > 0) // иначе нечего удалять
    {
        const size_type v = e.first;  // левая вершина
        const size_type w = e.second; // правая вершина
        // проверяем, существует ли удаляемое ребро
        if ((M[v][current_E] == 1) && (M[w][current_E] == 1))
        {
            --current_E;
            M[v][current_E] = M[w][current_E] = 0;
        }
    }
}

ostream& operator<<(ostream &t, const Graph_IM &r)
{
    string s;
    s = r.toString();
    return (t << s);
}

// вывод матрицы инцидентности
string Graph_IM::toString() const noexcept
{
    stringstream ss;
    // шапка матрицы (номера столбцов)
    ss << setw(3) << "\\" << setw(4) << "E" << setw(2) << "|" << endl;
    ss << setw(4) << "\\" << setw(5) << "|" << endl;
    ss << setw(2) << "V" << setw (3) << "\\" << setw(4) << "|";
    for (size_type k = 0; k < E; ++k) ss << "\t" << k+1;
    const string border_str(17 + 5 * E,'-');
    ss << endl << border_str << endl;
    // сама матрица и номера строк
    for (size_type i = 0; i < V; ++i)
    {
        ss << i+1 << "\t" << "|";
        for (size_type j = 0; j < E; ++j)
        {
            ss << "\t";
            ss << int(M[i][j]);
        }
        ss << endl << border_str << endl;
    }
    return ss.str();
}

// реализация методов итератора
Graph_IM::size_type Graph_IM::Iterator::operator++()
{
    // проходим по всем столбцам в матрице инцидентности (т.е проверяем все ребра)
    const size_type E = G.getE();
    for (++i; i < E; ++i)
    {
        // если нашли ребро, инцидентное вершине v, значит ищем вторую вершину, инцидентную этому ребру
        if (G.M[v][i] == 1)
        {
            // проходим по всем строкам данного столбца, чтобы найти вторую вершину
            const size_type V = G.getV();
            for (size_type j = 0; j < V; ++j)
            {
                if (G.M[j][i] == 1 && j != v) // когда нашли вторую вершину, и это не петля
                {
                    return j; // возвращаем индекс строки, т.е вторую вершину
                }
            }
        }
    }
    return NOT_FOUND; // если не нашли ничего, но в связном графе этого не должно происходить
}
