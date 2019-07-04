#include <windows.h>
#include <iostream>
#include <set>
#include <algorithm>

using namespace std;

int main()
{
    int *a = new int[5];
    a[0] = 3;
    a[1] = 4;
    a[2] = 5;
    a[3] = 6;
    a[4] = 2;
    a[5555] = 1;
    cout << a[5555];

}
