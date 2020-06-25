#include "object.h"

size_t Object::count = 0;

Object::Object()
{
    count++;
}

Object::~Object()
{
    count--;
}

size_t Object::Count() noexcept
{
    return count;
}
