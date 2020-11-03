#ifndef GRAPH_AM_H
#define GRAPH_AM_H

#include <vector>
#include <iostream>
#include "graph.h"
#include "graph_el.h"

using std::vector;

class Graph_AM : public Graph  // граф, представление - матрица смежности (Adjacency matrix)
                               // получается в ходе преобразования 2 из списка ребер (Graph_EL)
{
    using Row = vector<char>;   // в неор. графе если вершина смежна с другой вершиной, то 1, иначе 0
    using Matrix = vector<Row>;
// поля
    Matrix M; // матрица смежности V x V, V - число вершин, E - число ребер
public:
    // преобразование 2
    Graph_AM(const Graph_EL &_G) : Graph(_G.getV(),_G.getE()), M(V)
    {
        for (auto &row : M) row.resize(V,0);

        for (size_type v = 0; v < V; ++v)
        {
            Graph_EL::Iterator it(_G,v);
            for (size_type w = it.begin(); !it.end(); w = ++it)
            {
                if (v < w) insert({v,w});
            }
        }
    }
    virtual ~Graph_AM() = default;
    virtual void insert(Edge) noexcept;         // вставить ребро
    virtual void remove(Edge) noexcept;         // удалить ребро
    virtual std::string toString() const noexcept;      // вывести список ребер

    friend std::ostream& operator<<(std::ostream& t, const Graph_AM &r);

    // итератор: передаем какую-то вершину, и должны получить индекс другой вершины, соединенной с переданной вершиной ребром
    struct Iterator : public Graph::Iterator
    {
        const Graph_AM &G;
        explicit Iterator(const Graph_AM& _G, size_type _v) : Graph::Iterator(_G,_v), G(_G) { }
        Iterator() = delete;
        virtual size_type operator++();
        virtual bool end() { return i >= G.getV(); }
    };
};

#endif // GRAPH_AM_H
