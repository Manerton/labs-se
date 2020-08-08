#include <iostream>

using namespace std;
// если наследование в виде половина ромба, т.е от up -> left -> down, и up -> down, и вызывается метод print, который
// существует в up и в left, то возникает двусмысленность, о чем говорит комплилятор
// эта двусмысленность может разрулиться благодаря принципу (правилу) доминирования, согласно которому компилятор выбирает «наиболее производное» имя, которое считает доминирующим
// т.е вызовится метод print из класса Left
// правило работает только при виртуальном наследовании
class Up
{
public:
    virtual ~Up() {};
    virtual void Print() { cout << "Up!" << endl; };
};

class Left: virtual public Up
{
public:
    virtual void Print() { cout << "Left!" << endl; }
};

// если наследование в виде ромба, где up -> left -> down и up-> right -> down, причем в right метод print не переопределяется
// и как бы транзитом попадает из Up, то и в этом случае работает правило
//class Right: virtual public Up { };
//class Down: public Left, public Right { };
class Down: public Left, virtual public Up { };

int main()
{
    Down a;
    a.Print();
    return 0;
}
