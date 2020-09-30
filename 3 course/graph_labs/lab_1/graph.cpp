#include "graph.h"

using namespace std;

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
// по индексу index_for_E, т.е вставка осуществляется до E
void Graph::insert(Graph::Edge e)
{
    if (index_for_E < E) // уже некуда вставлять
    {
        const size_type v = e.first;  // левая вершина
        const size_type w = e.second; // правая вершина
        // проверяем, нет ли уже ребра по индексу index_for_E
        if (!((M[v][index_for_E] == 1) && (M[w][index_for_E] == 1)))
        {
            M[v][index_for_E] = M[w][index_for_E] = 1;
            ++index_for_E;
        }
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

void Graph::display()
{
    // шапка матрицы (номера столбцов)
    cout << setw(3) << "\\" << setw(4) << "E" << setw(2) << "|" << endl;
    cout << setw(4) << "\\" << setw(5) << "|" << endl;
    cout << setw(2) << "V" << setw (3) << "\\" << setw(4) << "|";
    for (size_type k = 0; k < E; ++k) cout << "\t" << k+1;
    const std::string border_str(60,'-');
    cout << endl << border_str << endl;
    // сама матрица и номера строк
    for (size_type i = 0; i < V; ++i)
    {
        cout << i+1 << "\t" << "|";
        for (size_type j = 0; j < E; ++j)
        {
            cout << "\t";
            cout << int(M[i][j]);
        }
        cout << endl << border_str << endl;
    }
}

Graph::size_type Graph::Iterator::operator++()
{
    // проходим по всем столбцам в матрице инцидентности (т.е проверяем все ребра)
    size_type E = G.getE();
    while (i < E)
    {
        // если нашли ребро, инцидентное вершине v, значит ищем вторую вершину, инцидентную этому ребру
        if (G.M[v][i] == 1)
        {
            // проходим по всем строкам данного столбца, чтобы найти вторую вершину
            size_type V = G.getV();
            for (size_type j = 0; j < V; ++j)
            {
                if (G.M[j][i] == 1 && j != i) // когда нашли вторую вершину, и это не петля
                {
                    ++i;
                    return j; // возвращаем индекс строки, т.е вторую вершину
                }
            }
        }
        else ++i;
    }
    return NOT_FOUND; // если не нашли ничего, но в связном графе этого не должно происходить
}

bool Graph::Iterator::end()
{
    return i >= G.getE();
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
