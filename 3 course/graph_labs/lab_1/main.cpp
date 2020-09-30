#include <iostream>
#include "graph.h"

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    Graph G{4,5};
    G.display();
    cout << endl;
    G.insert({0,1});
    G.insert({0,3});
    G.insert({1,3});
    G.insert({2,1});
    G.insert({3,2});
    G.display();
    return 0;
}
