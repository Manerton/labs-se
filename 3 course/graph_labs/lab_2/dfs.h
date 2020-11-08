#ifndef DFS_H
#define DFS_H
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

template <class Graph>
class DFS
{
    using size_type = typename Graph::size_type;
    static constexpr size_type NOT_VISITED = Graph::NOT_FOUND;
    size_type count = 0;        // число посещенных вершин
    const Graph &G;             // ссылка на граф
    std::vector<size_type> visited; // массив в котором вершины записываются в порядке их посещения

    void search(size_type v)
    {
        visited[v] = count++;
        typename Graph::Iterator it(G,v);
        for (size_type i = it.begin(); !it.end(); i = ++it)
        {
            std::cout << std::endl << v << "-" << i << " ";
            if (visited[i] == NOT_VISITED) search(i);
        }
    }

    std::string toString() const noexcept
    {
        std::stringstream ss;
        for (auto val : visited)
        {
            ss << val << " ";
        }
        return ss.str();
    }
public:
    DFS(const Graph &_G, size_type _v = 0) : G(_G), visited(G.getV(), NOT_VISITED)
    { search(_v); }

    size_type getCount() const noexcept { return count; }
    size_type operator[](size_type v) const noexcept { return visited[v]; }

    friend std::ostream& operator<<(std::ostream& t, const DFS &r)
    {
        std::string s;
        s = r.toString();
        return (t << s);
    }
};

#endif // DFS_H
