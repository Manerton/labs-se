#ifndef GRAPH_AL_H
#define GRAPH_AL_H

#include <vector>
#include <list>
#include "graph.h"
#include "graph_im.h"

class Graph_AL : public Graph  // граф, представление - список смежных вершин (вектор списков) (AL - Adjacency list)
{
    using List = std::list<size_type>;
    using AdjacencyList = std::vector<List>;
// поля
    AdjacencyList L;           // список cмежных вершин (вектор списков)
public:
    Graph_AL(size_type _V, size_type _E) : Graph(_V,_E), L(V) {}
    virtual ~Graph_AL() = default;
    virtual void insert(Edge) noexcept;             // вставить ребро
    virtual void remove(Edge) noexcept;             // удалить ребро
    virtual std::string toString() const noexcept;  // вывести список ребер

    friend std::ostream& operator<<(std::ostream& t, const Graph_AL &r);

    // итератор: передаем какую-то вершину, и должны получить индекс другой вершины, соединенной с переданной вершиной ребром
    struct Iterator : public Graph::Iterator
    {
        const Graph_AL &G;
        List::const_iterator i;
        explicit Iterator(const Graph_AL& _G, size_type _v) : Graph::Iterator(_G,_v), G(_G) { i = begin(); }
        Iterator() = delete;
        virtual size_type operator++();
        List::const_iterator begin() { return i = G.L[v].begin(); }
        virtual bool end() { return i == G.L[v].end(); }
    };
};

#endif // GRAPH_AL_H
