#include <iostream>
#include "graph.h"
#include "graph_im.h"
#include "graph_el.h"
#include "graph_am.h"
#include "degree.h"

using namespace std;

int main()
{
    Graph_IM G_IM{4,5};
    cout << "Display generated incidence matrix:" << endl;
    G_IM.insert({0,1});
    G_IM.insert({0,3});
    G_IM.insert({1,3});
    G_IM.insert({2,1});
    G_IM.insert({3,2});
    G_IM.display();
    cout << endl << "Degree of incidence matrix: ";
    Degree D1(G_IM);
    for (int i = 0; i < G_IM.getV(); ++i)
    {
        cout << D1[i] << " ";
    }
    cout << endl << "List of edges: ";
    Graph_EL list(G_IM);
    list.display();
    cout << endl << "Degree of list of edges: ";
    Degree D2(list);
    for (int i = 0; i < list.getV(); ++i)
    {
        cout << D2[i] << " ";
    }
    Graph_AM G_AM(G_IM);
    cout << endl << "Display adjacency matrix:" << endl;
    G_AM.display();
    cout << endl << "Degree of adjacency matrix: ";
    Degree D3(G_AM);
    for (int i = 0; i < G_AM.getV(); ++i)
    {
        cout << D3[i] << " ";
    }
    return 0;
}
