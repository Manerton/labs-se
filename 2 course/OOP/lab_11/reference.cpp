#include "reference.h"
#include "array.h"

// реализация l-value

reference &reference::operator=(const value_type &b)
{
    ref.data[index] = b;
    return *this;
}

reference &reference::operator+=(const reference::value_type &b)
{
    ref.data[index] += b;
    return *this;
}

reference &reference::operator-=(const reference::value_type &b)
{
    ref.data[index] -= b;
    return *this;
}

reference &reference::operator*=(const reference::value_type &b)
{
    ref.data[index] *= b;
    return *this;
}

reference &reference::operator/=(const reference::value_type &b)
{
    ref.data[index] /= b;
    return *this;
}

reference &reference::operator&=(const reference::value_type &b)
{
    ref.data[index] &= b;
    return *this;
}

reference &reference::operator|=(const reference::value_type &b)
{
    ref.data[index] |= b;
    return *this;
}

reference &reference::operator^=(const reference::value_type &b)
{
    ref.data[index] ^= b;
    return *this;
}

reference &reference::operator%=(const reference::value_type &b)
{
    ref.data[index] %= b;
    return *this;
}

// реализация l-value
reference &reference::operator++() noexcept
{
    ++ref.data[index];
    return *this;
}

reference reference::operator++(int) noexcept
{
    reference t = *this;
    ++(*this);
    return t;
}

reference &reference::operator--() noexcept
{
    --ref.data[index];
    return *this;
}

reference reference::operator--(int) noexcept
{
    reference t = *this;
    --(*this);
    return t;
}
// реализация r-value
reference::operator reference::value_type() const
{
    return ref.data[index];
}

const_reference::operator value_type() const
{
    return ref.data[index];
}
