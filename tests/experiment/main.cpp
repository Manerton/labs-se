#include <windows.h>
#include <iostream>
#include <set>
#include <algorithm>

using namespace std;

int main()
{
    int a = 5;
    cout << a;
    int *b = &a;
    cout << *b;
    int c = 3 + *b;
    cout << c;
    return 0;
}
