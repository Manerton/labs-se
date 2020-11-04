#ifndef GRAPH_EL_H
#define GRAPH_EL_H

#include <vector>
#include "graph.h"
#include "graph_im.h"

class Graph_EL : public Graph  // граф, представление - список ребер (вектор пар вершин) (EL - Edge list)
                               // получается в ходе преобразования 1 из матрицы инцидентности (Graph_IM)
{
    using List = std::vector<Edge>;
// поля
    List L;                             // список ребер (вектор пар вершин)
public:
    // преобразование 1
    Graph_EL(const Graph_IM &_G) : Graph(_G.getV(),_G.getE()), L(E)
    {
        for (size_type v = 0; v < V; ++v)
        {
            Graph_IM::Iterator it(_G,v);
            for (size_type w = it.begin(); !it.end(); w = ++it)
            {
                if (v < w) insert({v,w});
            }
        }
    }
    virtual ~Graph_EL() = default;
    virtual void insert(Edge) noexcept;         // вставить ребро
    virtual void remove(Edge) noexcept;         // удалить ребро
    virtual std::string toString() const noexcept;      // вывести список ребер

    friend std::ostream& operator<<(std::ostream& t, const Graph_EL &r);

    // итератор: передаем какую-то вершину, и должны получить индекс другой вершины, соединенной с переданной вершиной ребром
    struct Iterator : public Graph::Iterator
    {
        const Graph_EL &G;
        explicit Iterator(const Graph_EL& _G, size_type _v) : Graph::Iterator(_G,_v), G(_G) { }
        Iterator() = delete;
        virtual size_type operator++();
        virtual bool end() { return i >= G.getE(); }
    };
};

#endif // GRAPH_EL_H
