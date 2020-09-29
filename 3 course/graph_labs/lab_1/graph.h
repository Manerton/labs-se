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
    size_type V; // число вершин (кол-во строк)
    size_type E; // число ребер  (кол-во столбцов)
    Matrix M;    // матрица инцидентности
public:
    Graph(size_type _V, size_type _E);
    ~Graph();
    size_t getV() const; // получить число вершин
    size_t getE() const; // получить число ребер
    void insert(Edge);
    void remove(Edge);
    bool is_edge(size_type v, size_type w);
    // итератор
    struct Iterator
    {
        Iterator(int);
        int beg();
        int nxt();
        bool end();
    };

    // представления
    //vector<Edge> get_List_of_edges(); // список ребер (вектор пар вершин)
};

#endif // GRAPH_H
