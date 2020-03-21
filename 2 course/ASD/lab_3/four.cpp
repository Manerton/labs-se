#include "four.h"
#include <algorithm>
#include <iostream>
#include <numeric>

using namespace std;

void Four::CheckForQuaternary(const Array &arr) const
{
    bool b = all_of(arr.begin(),arr.end(),
                   [] (value_type c) {
                    return (c < num_sys);
                    });
    if (!b) throw not_quaternary_exception();
}

Four &Four::toAdditionalCode(size_type new_N)
{
    size_type diff = new_N - get_size();
    Four temp(new_N,num_sys-1);

    for (size_type i = diff; i < new_N; ++i)    // преобразую числа в соответствии с алгоритмом перевода в доп код -- //
    {
        temp[i] = num_sys-1 - (*this)[i-diff];
    }
    ++temp[new_N-1];
    *this = std::move(temp);
    return *this;
}

Four &Four::operator+=(const Four &b)
{
    Four max, min;  // -- прибавляю меньшее к большему -- //
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
    size_type N_max = max.get_size();

    reverse(max.begin(),max.end()); // -- переворачиваю -- //
    reverse(min.begin(),min.end()); // -- переворачиваю -- //

    size_type j = 0;
    for (size_type i = 0; i < N_max; ++i)
    {
        if (j < N_min) // -- если есть что прибавлять -- //
        {
            max[i] += min[j];
            ++j;
        }

        if (max[i] >= num_sys)
        {
            if (i == N_max - 1) {max.push_back(0);} // -- при сумме получился новый разряд, поэтому поместим его в начало (push_back так как потом переверну) -- //
            max[i] -= num_sys; ++max[i+1];
        } // сокращаем
    }
    reverse(max.begin(), max.end()); // -- переворачиваю обратно -- //
    *this = std::move(max);
    return *this;
}

Four &Four::operator-=(const Four &b)
{
    // -- если уменьшаемое меньше вычитаемого -- //
    if (*this < b) throw a_is_less_than_b_when_subtracted();

    size_type new_N = b.get_size();
    if (get_size() > new_N) new_N = get_size();
    Four add_code_b(b); // -- получаю доп код -- //
    add_code_b.toAdditionalCode(new_N);
    *this += add_code_b;    // -- складываю с доп кодом -- //

    auto it = begin()+1; // -- удаляю лишние нули в начале числа, а так же удаляю лишний разряд полученный в результате суммы с доп кодом -- //
    while ((*it) == 0 && it != end()-1) it++; // -- считаю количество этих нулей -- //
    erase(begin(),it); // -- удаляю их, если были -- //
    return *this;
}

Four &Four::operator*=(const Four &b)
{
    Four max, min;  // -- прибавляю меньшее к большему -- //
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
    size_type N_max = max.get_size();

    reverse(max.begin(),max.end()); // -- переворачиваю -- //
    reverse(min.begin(),min.end()); // -- переворачиваю -- //
    Four result(N_max);
    for (size_type j = 0; j < N_min; ++j)
    {
        Four temp(max);

        value_type vume = 0;
        for (size_type i = 0; i < N_max; ++i)
        {
            temp[i] *= min[j];
            temp[i] += vume;
            if (temp[i] >= num_sys)
            {
                vume = (temp[i] / num_sys);
                if (i == N_max - 1) {temp.push_back(vume);} // -- получился новый разряд, поэтому поместим его в начало (push_back так как потом переверну) -- //
                temp[i] %= num_sys;
            } // сокращаем
            else vume = 0;
        }

        Four temp_res(temp.get_size()+j);
        std::reverse_copy(temp.begin(),temp.end(),temp_res.begin());
        result += temp_res;
    }
    *this = std::move(result);
    return *this;
}

Four& Four::operator/=(const Four &b)
{
    Four res;

    if (b.get_size() == 1 && b[0] == 0) throw zero_divide_exception();

    if (*this == b)
    {
        res = {1};
    }
    else if (*this < b)
    {
        res = {0};
    }
    else
    {
        Four one(1,1);
        while (*this >= b) {*this -= b; res+=one;}
    }

    *this = std::move(res);
    return *this;
}

Four operator-(const Four &a, const Four &b)
{
    Four t = a;
    t -= b;
    return t;
}

Four operator+(const Four &a, const Four &b)
{
    Four t = a;
    t += b;
    return t;
}

Four operator*(const Four &a, const Four &b)
{
    Four t = a;
    t *= b;
    return t;
}

Four operator/(const Four &a, const Four &b)
{
    Four t = a;
    t /= b;
    return t;
}







