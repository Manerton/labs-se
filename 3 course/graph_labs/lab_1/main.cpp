#include <iostream>
#include "graph_im.h"

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    Graph_IM G{4,5};
    G.display();
    cout << endl;
    G.insert({0,1});
    G.insert({0,3});
    G.insert({1,3});
    G.insert({2,1});
    G.insert({3,2});
    G.display();
    cout << endl << "List of edges: ";
    auto list = G.get_List_of_edges();
    for (const auto &val : list)
    {
        cout << "(" << val.first+1 << "; " << val.second+1 << ")" << " ";
    }
    return 0;
}
