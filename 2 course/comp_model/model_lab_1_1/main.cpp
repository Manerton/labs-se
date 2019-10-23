#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> a = {3, 7, 9};
    for (auto var : a) cout << var;
    a();

    return 0;
}
