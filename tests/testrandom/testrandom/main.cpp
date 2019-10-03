#include <iostream>
#include <vector>

using namespace std;

void sort(int A[])
{
    cout << A << endl;
    int n = 7;
    int x;
    for (int i = 0; i < n; i++)
    {
        x = A[i];
        int j = i;
        while ((j > 0) && (x < A[j-1]))
        {
            A[j] = A[j-1];
            --j;
        }
        A[j] = x;
    }
}

int main()
{
    int A[7]={1, 3, 5, 4, 5, 6, 7};
    cout << A << endl;

    sort(A);
    cout << A << endl;
    for (int i = 0; i < 7; i++)
    {
        cout << A[i] << " ";
    }
    return 0;
}
