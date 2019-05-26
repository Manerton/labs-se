#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>

using namespace std;

typedef vector<int> row;
typedef vector<row> matrix;

void display (const matrix &M)
{
    int N = M.size();
    for (int i = 0; i < N; i++)
    {
        for (auto &c : M[i]) cout << c << " ";
        cout << endl;
    }
}

void display (const row &X)
{
    for (auto &c : X) cout << c << " ";
}

int main()
{
    system("chcp 1251 > nul");
    matrix M;
    row A {5, 4, 3, 1, 6, 4, 8};
    row B {5, 2, 2, 1, 4, 2};
    M.push_back(A);
    M.push_back(A);
    M.push_back(A);
    M.push_back(B);
    M.push_back(A);
    display(M);
    cout << endl;
    matrix newM (M.end()-2, M.end());
    display(newM);

    //matrix TEST = M;
    //display(TEST);

    return 0;
}
