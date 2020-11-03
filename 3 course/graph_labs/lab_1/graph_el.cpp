#include <algorithm>
#include <sstream>
#include "graph_el.h"
using namespace std;

// вставить ребро
void Graph_EL::insert(Graph_EL::Edge e) noexcept
{
    if (current_E < E) // иначе некуда вставлять
    {
        L[current_E] = e;
        ++current_E;
    }
}

// удалить ребро
void Graph_EL::remove(Graph_EL::Edge e) noexcept
{
    // проверяем, существует ли удаляемое ребро
    if (find(L.begin(),L.end(),e) != L.end())
    {
        L.erase(L.begin()+current_E);
        --current_E;
    }
}

// вывести список ребер
string Graph_EL::toString() const noexcept
{
    stringstream ss;
    for (auto &val : L)
    {
        ss << "(" << val.first+1 << "; " << val.second+1 << ")" << endl;
    }
    return ss.str();
}

ostream& operator<<(ostream &t, const Graph_EL &r)
{
    string s;
    s = r.toString();
    return (t << s);
}

Graph_EL::size_type Graph_EL::Iterator::operator++()
{
    // проходим по всему списку ребер (по всему вектору)
    const size_type E = G.getE();
    for (++i; i < E; ++i)
    {
        // если нашли ребро, инцидентное вершине v, значит ищем вторую вершину, инцидентную этому ребру
        if (G.L[i].first == v)
        {
            return G.L[i].second;
        }
        else if (G.L[i].second == v)
        {
            return G.L[i].first;
        }
    }
    return NOT_FOUND; // если не нашли ничего, но в связном графе этого не должно происходить
}
