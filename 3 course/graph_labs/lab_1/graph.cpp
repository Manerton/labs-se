#include "graph.h"

Graph::Graph(Graph::size_type _V, Graph::size_type _E) : V(_V), E(_E)
{
    // инициализируем пустую матрицу V x E
    M.resize(V);
    for (auto &row : M) row.resize(E,0);
}

Graph::size_type Graph::getV() const
{
    return V;
}

Graph::size_type Graph::getE() const
{
    return E;
}

// вставка ребер в матрицу V x E
// по индексу inserted_E, т.е вставка осуществляется до E
void Graph::insert(Graph::Edge e)
{
    const size_type v = e.first;  // левая вершина
    const size_type w = e.second; // правая вершина
    // проверяем, нет ли уже ребра по индексу inserted_E
    if ((M[v][index_for_E] == 0) && (M[w][index_for_E] == 0))
    {
        ++index_for_E;
        M[v][index_for_E] = M[w][index_for_E] = 1;
    }
}

void Graph::remove(Graph::Edge e)
{
    const size_type v = e.first;  // левая вершина
    const size_type w = e.second; // правая вершина
    // проверяем, существует ли удаляемое ребро
    if ((M[v][index_for_E] == 1) && (M[w][index_for_E] == 1))
    {
        --index_for_E;
        M[v][index_for_E] = M[w][index_for_E] = 0;
    }
}

//vector<Graph::Edge> Graph::get_List_of_edges()
//{
//    size_t E = 0; // количество рёбер

//    const auto G_E = this->E();
//    const auto G_V = this->V();

//    vector<Edge> a(G_E);

//    for (size_t v = 0; v < G_V; ++v)
//    {
//        Graph::Iterator it(v);
//        for (size_t w = it.beg(); !it.end(); w = it.nxt())
//        {
//            if(v < w)
//            {
//                a[E++] = Edge(v,w);
//            }
//        }
//    }
//}

Graph::size_type Graph::Iterator::operator++()
{
    while (i < G.getV())
    {
        if (G.M[v][i] == true) return i;
    }
    // надо думать return 0;
}
