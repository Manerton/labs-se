#include "bitstring.h"
#include <algorithm>
#include <iostream>
#include <numeric>

using namespace std;

void BitString::CheckForBinary(const Array &arr) const
{
    bool b = all_of(arr.begin(),arr.end(),
                   [] (value_type c) {
                    return (c < num_sys);
                    });
    if (!b) throw not_binary_exception();
}

int64_t BitString::count_of_SingleBit() const noexcept
{
    auto res = count(begin(), end(), 1);
    return res;
}

BitString &BitString::operator&=(const BitString &b)
{
    BitString max, min;
    if (get_size() < b.get_size())
    {
        max = b;
        min = *this;
    }
    else
    {
        max = *this;
        min = b;
    }
    size_type N_min = min.get_size();

    reverse(max.begin(),max.end()); // -- переворачиваю -- //
    reverse(min.begin(),min.end()); // -- переворачиваю -- //

    for (size_type i = 0; i < N_min; ++i)
    {
        min[i] &= max[i];
    }
    reverse(min.begin(), min.end()); // -- переворачиваю обратно -- //
    *this = std::move(min);
    return *this;
}

BitString &BitString::operator|=(const BitString &b)
{
    BitString max, min;
    if (get_size() < b.get_size())
    {
        max = b;
        min = *this;
    }
    else
    {
        max = *this;
        min = b;
    }
    size_type N_min = min.get_size();

    reverse(max.begin(),max.end()); // -- переворачиваю -- //
    reverse(min.begin(),min.end()); // -- переворачиваю -- //

    for (size_type i = 0; i < N_min; ++i)
    {
        max[i] |= min[i];
    }
    reverse(max.begin(), max.end()); // -- переворачиваю обратно -- //
    *this = std::move(max);
    return *this;
}

BitString &BitString::operator^=(const BitString &b)
{
    BitString max, min;
    if (get_size() < b.get_size())
    {
        max = b;
        min = *this;
    }
    else
    {
        max = *this;
        min = b;
    }
    size_type N_min = min.get_size();

    reverse(max.begin(),max.end()); // -- переворачиваю -- //
    reverse(min.begin(),min.end()); // -- переворачиваю -- //

    for (size_type i = 0; i < N_min; ++i)
    {
        max[i] ^= min[i];
    }
    reverse(max.begin(), max.end()); // -- переворачиваю обратно -- //
    *this = std::move(max);
    return *this;
}

BitString& BitString::operator<<=(uint64_t i)
{
    BitString new_string(get_size()+i);
    std::move(begin(),end(),new_string.begin());
    *this = std::move(new_string);
    return *this;
}

BitString& BitString::operator>>=(uint64_t i)
{
    BitString new_string(get_size()+i);
    std::move(begin(),end(),new_string.begin()+i);
    *this = std::move(new_string);
    return *this;
}

BitString BitString::operator~()
{
    BitString t(*this);
    for (value_type &var : t)
    {
        var ^= 1;
    }
    return t;
}

BitString operator&(const BitString &a, const BitString &b)
{
    BitString t = a;
    t &= b;
    return t;
}

BitString operator|(const BitString &a, const BitString &b)
{
    BitString t = a;
    t |= b;
    return t;
}

BitString operator^(const BitString &a, const BitString &b)
{
    BitString t = a;
    t ^= b;
    return t;
}

BitString operator<<(const BitString &a, const uint8_t &b)
{
    BitString t = a;
    t <<= b;
    return t;
}

BitString operator>>(const BitString &a, const uint8_t &b)
{
    BitString t = a;
    t >>= b;
    return t;
}

bool operator<(const BitString &a, const BitString &b)
{
    return (a.count_of_SingleBit() < b.count_of_SingleBit());
}
bool operator>=(const BitString &a, const BitString &b)
{
    return !(a<b);
}

bool operator>(const BitString &a, const BitString &b)
{
    return (b < a);
}

bool operator<=(const BitString &a, const BitString &b)
{
    return !(b<a);
}






