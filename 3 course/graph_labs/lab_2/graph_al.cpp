#include <algorithm>
#include <sstream>
#include <iterator>
#include "graph_al.h"

using namespace std;

void Graph_AL::insert(Graph::Edge e) noexcept
{
    if (current_E < E) // иначе некуда вставлять
    {
        const size_type v = e.first;
        const size_type w = e.second;
        L[v].push_back(w);
        L[w].push_back(v);
        ++current_E;
    }
}

void Graph_AL::remove(Graph::Edge e) noexcept
{
    const size_type v = e.first;
    const size_type w = e.second;
    const auto find_res1 = find(L[v].begin(), L[v].end(), w);
    const auto find_res2 = find(L[w].begin(), L[w].end(), v);
    // проверяем, существует ли удаляемое ребро
    if ( ( find_res1 != L[v].end()) && ( find_res2 != L[w].end()) )
    {
        L[v].erase(find_res1);
        L[w].erase(find_res2);
        --current_E;
    }
}

string Graph_AL::toString() const noexcept
{
    stringstream ss;
    for (size_type i = 0; i < V; ++i)
    {
        ss << i+1 << ": ";
        for (auto it = L[i].cbegin(); it != L[i].end(); ++it)
        {
            ss << (*it)+1;
            if (*it != L[i].back()) ss << ", ";
        }
        ss << endl;
    }
    return ss.str();
}

ostream& operator<<(ostream &t, const Graph_AL &r)
{
    string s;
    s = r.toString();
    return (t << s);
}

Graph::size_type Graph_AL::Iterator::operator++()
{
    if (!end())
    {
        i++;
        size_type res = *i;
        return res;
    }
    return NOT_FOUND;
}
