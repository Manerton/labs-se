#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <iostream>
using std::vector;

class Graph_IM  // граф, представление - матрица инцидентности (IM - Incidence matrix)
{
    using size_type = uint16_t;                   // тип для нумерации вершин и ребер
    using Edge = std::pair<size_type, size_type>; // ребро - номера левой и правой вершины
    using Row = vector<char>;                     // в неор. графе если вершина инцидентна ребру, то 1, иначе 0
    using Matrix = vector<Row>;
    // NOT_FOUND это аналог i = -1 в signed (т.е используется как несуществующий индекс, и аналогично при ++i получится i = 0)
    static constexpr size_type NOT_FOUND = std::numeric_limits<int16_t>::max();
// поля
    size_type V = 0;            // число вершин (кол-во строк)
    size_type E = 0;            // общее число ребер в графе (кол-во столбцов)
    size_type index_for_E = 0;  // число ребер в графе в данный момент (используется как индекс при вставке ребра при генерации матрицы)
    Matrix M;                   // матрица инцидентности
public:
    Graph_IM(size_type _V, size_type _E);
    ~Graph_IM() = default;
    size_type getV() const noexcept;     // получить число вершин
    size_type getE() const noexcept;     // получить число ребер
    void insert(Edge) noexcept;          // вставить ребро
    void remove(Edge) noexcept;          // удалить ребро
    void display() const noexcept;       // вывести матрицу инцидентности

    // итератор: передаем вершину, должны получить индекс другой вершины, соединенной с переданной вершиной ребром
    struct Iterator
    {
        const Graph_IM &G;
        size_type v = 0;
        size_type i = NOT_FOUND;
        explicit Iterator(const Graph_IM& _G, size_type _v) : G(_G),v(_v) { }
        Iterator() = delete;
        size_type operator++();
        // начинаем идти с индекса i = NOT_FOUND, возвращаем первую смежную вершину для v
        size_type begin() { i = NOT_FOUND; return operator++(); }
        bool end() { return i >= G.getE(); }
    };

    // представления
    vector<Edge> get_List_of_edges() noexcept; // список ребер (вектор пар вершин)
};

#endif // GRAPH_H
