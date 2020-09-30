#ifndef GRAPH_H
#define GRAPH_H
#include <cstdint>
#include <vector>
using std::vector;

class Graph         // граф, представление - матрица инцидентности
{
    using size_type = uint16_t;                   // для нумерации вершин и ребер
    using Edge = std::pair<size_type, size_type>; // ребро - номера левой и правой вершины
    using Row = vector<char>;
    using Matrix = vector<Row>;
// поля
    size_type V = 0;           // число вершин (кол-во строк)
    size_type E = 0;           // общее число ребер в графе (кол-во столбцов)
    size_type index_for_E = 0; // число ребер в графе в данный момент (используется как индекс при вставке ребра при генерации матрицы)
    Matrix M;                  // матрица инцидентности
public:
    Graph(size_type _V, size_type _E);
    ~Graph();
    size_type getV() const; // получить число вершин
    size_type getE() const; // получить число ребер
    void insert(Edge);      // вставить ребро
    void remove(Edge);      // удалить ребро
    // итератор
    struct Iterator
    {
        const Graph &G;
        size_type v = 0;
        size_type i = 0;
        explicit Iterator(const Graph& _G, size_type _v) : G(_G),v(_v) { }
        Iterator() = delete;
        size_type operator++();
        bool end();
    };

    // представления
    //vector<Edge> get_List_of_edges(); // список ребер (вектор пар вершин)
};

#endif // GRAPH_H
