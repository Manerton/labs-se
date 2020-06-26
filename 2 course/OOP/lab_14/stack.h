#ifndef STACK_H
#define STACK_H

#include <cstdint>

class List;
class UnemployedElem;
class Stack
{
public:
    // конструкторы
    Stack();
    ~Stack();
    Stack(const Stack& other);
    Stack(Stack&& other) noexcept;              // -- конструктор переноса -- //
    Stack& operator=(const Stack& other);
    Stack& operator=(Stack&& other) noexcept;   // -- операция перемещения -- //
    // методы стека
    bool empty() noexcept;
    void clear();
    size_t size();
    UnemployedElem top();
    void push(const UnemployedElem&);
    void pop();
    // сравнение на равенство
    friend bool operator==(const Stack &a, const Stack &b);
    friend bool operator!=(const Stack &a, const Stack &b);
private:
    List* pimpl;
};

#endif // STACK_H
