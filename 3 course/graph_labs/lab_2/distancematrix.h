#ifndef DISTANCEMATRIX_H
#define DISTANCEMATRIX_H
#include <vector>
#include "bfs.h"
template <class Graph>
class DistanceMatrix
{
    using size_type = typename Graph::size_type;
    using Row = std::vector<size_type>;
    using Matrix = std::vector<Row>;

    const Graph &G;         // ссылка на граф
    Matrix STM;             // ST Matrix из BFS (матрица, где строка i - дерево BFS для соответствующей вершины i)
    Matrix DM;              // Distance Matrix  (матрица расстояний)
    size_type diameter = 0; // диаметр графа - максимальный из расстояний

    std::string toString() const noexcept
    {
        std::stringstream ss;
        for (auto &row : DM)
        {
            for (auto &cell : row)
            {
                ss << cell << " ";
            }
            ss << std::endl;
        }
        return ss.str();
    }

public:
    DistanceMatrix(const Graph &_G) : G(_G), STM(G.getV()), DM(G.getV())
    {
        const size_type V = G.getV();
        // инициализируем матрицу расстояний нулями
        for (auto &row : DM) row.resize(V,0);

        // инициализируем матрицу STM с помощью DFS (обход в ширину) для каждой из вершин
        for (size_type i = 0; i < V; ++i)
        {
            BFS bfs(G,i);
            STM[i] = bfs.getST_array();
        }

        // рассчитываем длины кратчайших путей из v в w
        for (size_type v = 0; v < V; ++v)
        {
            for (size_type w = 0; w < V; ++w)
            {
                size_type length = 0;   // длина пути
                // вычисляем ее с помощью матрицы STM
                for (size_type t = v; t != w; t = STM[w][t]) ++length;
                if (length > diameter) diameter = length;
                DM[v][w] = length;
            }
        }
    }

    size_type getDiameter() const { return diameter; }

    friend std::ostream& operator<<(std::ostream& t, const DistanceMatrix &r)
    {
        std::string s;
        s = r.toString();
        return (t << s);
    }

};

#endif // DISTANCEMATRIX_H
