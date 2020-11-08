#ifndef GRAPH_IM_H
#define GRAPH_IM_H
#include <vector>
#include "graph.h"

class Graph_IM : public Graph   // граф, представление - матрица инцидентности (IM - Incidence matrix)
                                // генерируется изначально
{
    using Row = std::vector<char>;   // в неор. графе если вершина инцидентна ребру, то 1, иначе 0
    using Matrix = std::vector<Row>;
// поля
    Matrix M; // матрица инцидентности V x E, V - число вершин (кол-во строк), E - число ребер (кол-во столбцов)
public:
    Graph_IM(size_type _V, size_type _E) : Graph(_V,_E), M(V)
    {
        // инициализируем пустую (заполненную нулями) матрицу V x E
        for (auto &row : M) row.resize(E,0);
    }
    virtual ~Graph_IM() = default;
    virtual void insert(Edge) noexcept;             // вставить ребро
    virtual void remove(Edge) noexcept;             // удалить ребро
    virtual std::string toString() const noexcept;          // вывести матрицу инцидентности

    friend std::ostream& operator<<(std::ostream& t, const Graph_IM &r);

    // итератор: передаем какую-то вершину, и должны получить индекс другой вершины, соединенной с переданной вершиной ребром
    struct Iterator : public Graph::Iterator
    {
        const Graph_IM &G;
        size_type i = NOT_FOUND;
        explicit Iterator(const Graph_IM& _G, size_type _v) : Graph::Iterator(_G,_v), G(_G) { }
        Iterator() = delete;
        virtual size_type operator++();
        virtual size_type begin() { i = NOT_FOUND; return operator++(); }
        virtual bool end() { return i >= G.getE(); }
    };
};

#endif // GRAPH_IM_H
