#include <iostream>

using namespace std;

struct A
{
    int member;
    A(int param)
    {
        member = param;
    }
};

// если убрать виртуал, то не получится сделать C() : A(7)
// при виртуал придется явно инициализиовать в C как то значение для базового A, о чем говорит компилятор (или clang, в общем IDE)
// иначе говоря
// когда класс B имеет виртуальную базу A это означает, что A будет находиться в большинстве производных классов дерева наследования и,
// следовательно, большинство производных классов также отвечает за инициализацию этой виртуальной базы:
struct B: virtual A
{
    B(): A(5){}
};

struct C: B
{
    C() : A(7) {}
};


// проблема алмаза (the diamond problem)
//     A                                        A   A
//   /  \                                       |   |
//  B   C                                      B   C
//    |                                        \ /
//   D                                          D
// virtual inheritance                   normal inheritance

struct A2
{
    void foo() {}
};

struct B2 : public virtual A2 {};
struct C2 : public virtual A2 {};

struct D2 : public B2, public C2
{
    void bar()
    {
        foo(); // если наследование не virtual, то ошибка, чей foo? B2::foo() or C2::foo()? - двусмысленность
    }
};

int main()
{
    C a;
    int b;
    cin >> b;
    return 0;
}
