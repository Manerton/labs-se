#include "graph.h"

Graph::Graph(Graph::size_type _V, Graph::size_type _E) : V(_V), E(_E)
{
    M.resize(V);
    for (auto &row : M) row.resize(E,0);
}

size_t Graph::getV() const
{
    return V;
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
