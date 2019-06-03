#include <windows.h>
#include <iostream>

using namespace std;

bool op_or (bool x, bool y, bool z)
{
    bool result = x
    return (x oper ((y oper z) oper (x oper y)));  // - ÈËÈ
}
int main()
{
    system("chcp 1251 > nul");
    bool x (0), y (0), z(1);
    cout << op_or(x,y,z);
}
