#ifndef BFS_H
#define BFS_H
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <queue>

template <class Graph>
class BFS   // обход в ширину
{
    using size_type = typename Graph::size_type;
    using Edge = typename Graph::Edge;
    static constexpr size_type NOT_VISITED = Graph::NOT_FOUND;
    size_type count = 0;            // число посещенных вершин
    const Graph &G;                 // ссылка на граф
    std::vector<size_type> visited; // массив в котором вершины записываются в порядке их посещения

    // представление дерева BFS в виде родительских связей, которое можно использовать при
    // поиске кратчайшего пути из любого узла в корень.
    // Например, 3-5-0 - это кратчайший путь в графе из 3 в 0, поскольку st[3] = 5, а st[5] = 0.
    std::vector<size_type> st;

    // вершины маркируются в момент постановки их в очередь
    void search(Edge e)
    {
        std::queue<Edge> Q;
        Q.push(e);
        visited[e.second] = count++;
        while (!Q.empty())
        {
            e = Q.front();
            Q.pop();
            const size_type v = e.first;  // левая вершина
            const size_type w = e.second; // правая вершина
            st[w] = v;
            typename Graph::Iterator it(G,w);
            for (size_type i = it.begin(); !it.end(); i = ++it)
            {
                if (visited[i] == NOT_VISITED)
                {
                    Q.push({w,i});
                    visited[i] = count++;
                }
            }
        }
    }

    std::string toString() const noexcept
    {
        std::stringstream ss;
        for (auto val : visited)
        {
            ss << val+1 << " ";
        }
        return ss.str();
    }
public:
    BFS(const Graph &_G, size_type _v = 0) : G(_G), visited(G.getV(), NOT_VISITED), st(G.getV(), NOT_VISITED)
    { search({_v,_v}); }

    size_type getCount() const noexcept { return count; }
    size_type getST(size_type v) const noexcept { return st[v]; }
    std::vector<size_type> getST_array() const noexcept { return st; }
    size_type operator[](size_type v) const noexcept { return visited[v]; }

    friend std::ostream& operator<<(std::ostream& t, const BFS &r)
    {
        std::string s;
        s = r.toString();
        return (t << s);
    }
};

#endif // BFS_H
