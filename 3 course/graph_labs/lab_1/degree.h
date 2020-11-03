#ifndef DEGREE_H
#define DEGREE_H
#include <vector>
#include <numeric>
#include <cassert>
#include <iostream>
#include <sstream>
#include <algorithm>
using std::vector;

template <class Graph>
class Degree    // класс, определяющий степени вершин
{
    using size_type = typename Graph::size_type; // тип для степени

    const Graph &G;
    vector<size_type> degree;
public:
    Degree(const Graph &_G) : G(_G), degree(G.getV(), 0)
    {
        const size_type V = G.getV();
        for (size_type v = 0; v < V; ++v)
        {
            typename Graph::Iterator it(G,v);
            for (it.begin(); !it.end(); ++it)
            {
                degree[v]++;
            }
        }
        size_type sum = std::accumulate(degree.begin(), degree.end(), 0);
        // проверка 1-ой теоремы о степенях вершин в графе, а именно:
        // Сумма степеней вершин в неориентированном графе четна и равна удвоенному числу ребер.
        assert((sum % 2 == 0) && (sum == 2 * G.getE()));

        size_type count = std::count_if(degree.begin(),degree.end(), [](size_type d){return d % 2 == 1;} );
        // проверка 2-ой теоремы о степенях вершин в графе, а именно:
        // Число вершин нечетной степени в любом графе четно.
        assert(count % 2 == 0);
    }
    size_type operator[](int v) const { return degree[v]; }

    std::string toString() const noexcept
    {
        std::stringstream ss;
        for (auto val : degree)
        {
            ss << val << " ";
        }
        return ss.str();
    }
    friend std::ostream& operator<<(std::ostream& t, const Degree &r)
    {
        std::string s;
        s = r.toString();
        return (t << s);
    }
};

#endif // DEGREE_H
