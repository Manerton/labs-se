#ifndef GRAPH_H
#define GRAPH_H
#include <cstdint>
#include <string>
#include <limits>
#include <vector>
using std::vector;

class Graph     // абстрактный класс графа
{
public:
    using size_type = uint16_t;                   // тип для нумерации вершин и ребер
    using Edge = std::pair<size_type, size_type>; // ребро - номера левой и правой вершины
    // NOT_FOUND это аналог i = -1 в signed (т.е используется как несуществующий индекс, и аналогично при ++i получится i = 0)
    static constexpr size_type NOT_FOUND = std::numeric_limits<uint16_t>::max();
protected:
// поля
    // количество вершин и ребер генерируются перед формированием графов
    size_type V = 0;                    // число вершин
    size_type E = 0;                    // общее число ребер в графе, сколько должно быть в конечном счете после генерации
    size_type current_E = 0;            // число ребер в графе в данный момент (используется как индекс при вставке ребра при генерации матрицы)
public:
    Graph(size_type _V, size_type _E) : V(_V), E(_E) {}
    virtual ~Graph() {};
    size_type getV() const noexcept {return V;}     // получить число вершин
    size_type getE() const noexcept {return E;}     // получить число ребер
    virtual void insert(Edge) noexcept = 0;         // вставить ребро
    virtual void remove(Edge) noexcept = 0;         // удалить ребро
    virtual std::string toString() const noexcept = 0;      // вывести граф

    // итератор: передаем какую-то вершину, и должны получить индекс другой вершины, соединенной с переданной вершиной ребром
    struct Iterator
    {
        const Graph &G;        // ссылка на наш граф
        const size_type v;     // в эту переменную помещаем номер переданной вершины
        size_type i = NOT_FOUND;

        explicit Iterator(const Graph& _G, size_type _v) : G(_G),v(_v) { }
        Iterator() = delete;

        virtual size_type operator++() = 0;
        // начинаем идти с индекса i = NOT_FOUND, возвращаем первую смежную вершину для v
        size_type begin() { i = NOT_FOUND; return operator++(); }
        virtual bool end() = 0;
    };
};

#endif // GRAPH_H
