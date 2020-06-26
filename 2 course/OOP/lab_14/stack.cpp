#include "stack.h"
#include "list.h"

Stack::Stack() : pimpl(new List) {}

Stack::~Stack()
{
    delete pimpl;
}

Stack::Stack(const Stack &other) : pimpl (new List(*other.pimpl)){}

Stack::Stack(Stack &&other) noexcept : pimpl (new List(std::move(*other.pimpl)))
{

}

Stack &Stack::operator=(const Stack &other)
{
    pimpl->operator=(*other.pimpl);
    return *this;
}

Stack &Stack::operator=(Stack &&other) noexcept
{
    pimpl->operator=(std::move(*other.pimpl));
    return *this;
}

bool Stack::empty() noexcept
{
    bool res = pimpl->empty();
    return res;
}

void Stack::clear()
{
    pimpl->clear();
}

size_t Stack::size()
{
    return pimpl->size();
}

UnemployedElem Stack::top()
{
    return pimpl->back();
}

void Stack::push(const UnemployedElem &b)
{
    pimpl->push_back(b);
}

void Stack::pop()
{
    pimpl->pop_back();
}

bool operator==(const Stack &a, const Stack &b)
{
    return *(a.pimpl) == *(b.pimpl);
}

bool operator!=(const Stack &a, const Stack &b)
{
    return !(a == b);
}
