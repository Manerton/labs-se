#include <iostream>
#include <vector>
#include <chrono>


using namespace std;
using namespace std::chrono;
using sec = duration<double, ratio<1>>;

void somefunc(vector<int> &v)
{
    for (int i = 0; i < 1000000000; ++i)
    {
        int a = v[99];
        int b = v[99] * v[99];
        int c = a - b;
    }
}

int main()
{
    cout << "Hello World!" << endl;
    vector<int> vec(100,8);
    const auto start = chrono::steady_clock::now();
    somefunc(vec);
    const auto end = chrono::steady_clock::now();
    const auto res = duration_cast<sec>(end - start);
    cout << res.count();
    return 0;
}
