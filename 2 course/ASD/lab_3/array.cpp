#include "array.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

void Array::CheckZeroSize(size_type n) const
{
    if (!n) throw zero_array_size_exception();
}

void Array::CheckIndex(size_type index) const
{
    if (index >= size_array) throw std::out_of_range{"bad index exception"};
}

std::string Array::toString() const noexcept
{
    if (size_array != 0)
    {
        std::stringstream ss;
        for (int val : (*this))
        {
            ss << val << " ";
        }
        std::string s(ss.str());
        s.pop_back(); // -- удаляю лишний пробел -- //
        return s;
    }
    return "empty";
}

Array::Array(size_type n, value_type t)
{
    CheckZeroSize(n);
    size_array = n;
    data = new value_type[size_array];
    std::fill(begin(),end(),t);
}

Array::Array(const std::initializer_list<value_type> &t) : Array(t.size())   // -- делегируем конструктуру выше -- //
{
    std::copy(t.begin(),t.end(),begin());
}

Array::Array(const std::string &str) : Array(str.length())
{
    size_type i = 0;
    for (char var : str)
    {
        data[i] = static_cast<value_type>(var - '0');
        ++i;
    }
}

Array::Array(const Array &other) : size_array(other.size_array), data(new value_type[size_array])
{
    std::copy(other.begin(),other.end(),begin());
}

Array::Array(Array &&other) noexcept : size_array(other.size_array), data(other.data)
{
    other.size_array = 0;
    other.data = nullptr;
}

Array &Array::operator=(const Array &other) // -- по Саттеру -- //
{
    if (this != &other)
    {
        Array temp(other);
        std::swap(size_array,temp.size_array);
        std::swap(data,temp.data);
    }
    return *this;
}

Array &Array::operator=(Array &&other) noexcept
{
    if (this != &other)
    {
        std::swap(size_array,other.size_array);
        std::swap(data,other.data);
    }
    return *this;
}

Array::reference Array::operator[](size_type index)
{
    CheckIndex(index);
    return data[index];
}

Array::const_reference Array::operator[](size_type index) const
{
    CheckIndex(index);
    return data[index];
}

void Array::push_back(value_type b)
{
    Array t(size_array+1, b);   // -- создаю новый массив, у которого размер больше на 1 элемент -- //
    std::move(begin(),end(),t.begin()); // -- перемещаю элементы из старого массива -- //
    *this = std::move(t);   // -- вызываю конструктор перемещения, меняю t и this местами, старый удаляется при выходе из этого метода -- //
}

void Array::pop_back()
{
    --size_array; // -- возможно дешевле будет этот элемент не освобождать -- //
}

Array::value_type* Array::erase(value_type *first, value_type *last)
{
    if (first >= begin() && last <= end() && last > first)  // -- проверки на выход за диапазон -- //
    {
        size_type distance = size_type(last-first); // -- диапазон удаляемых элементов -- //
        Array t;
        if (size_array != distance) // -- если удаляем не всё -- //
        {
            t = Array(size_array - distance);
        }
        size_type pos = t.get_size();   // -- смещение для указателя, следующего за последним удаленным элементом -- //
        if (begin() != first)
        {
            std::move(begin(),first,t.begin());
        }
        if (end() != last)
        {
            pos = size_type(first-begin());
            std::move(last,end(),t.begin()+pos);
        }
        *this = std::move(t);
        return (begin()+pos);
    }
    return nullptr;
}

bool operator==(const Array &a, const Array &b)
{
    if (a.size_array != b.size_array) return false;
    return std::equal(a.begin(),a.end(),b.begin());
}

bool operator!=(const Array &a, const Array &b)
{
    return !(a == b);
}

bool operator<(const Array &a, const Array &b)
{
    if (a.get_size() == b.get_size())
    {
        return std::lexicographical_compare(a.begin(),a.end(),b.begin(),b.end());
    }
    return (a.get_size() < b.get_size());
}

bool operator>=(const Array &a, const Array &b)
{
    return !(a < b);
}

bool operator>(const Array &a, const Array &b)
{
    return (b < a);
}

bool operator<=(const Array &a, const Array &b)
{
    return !(b < a); // тоже реализую через одну операцию <
}

std::ostream& operator<<(std::ostream& t, const Array &r)
{
    std::string s = r.toString();
    return (t << s);
}
std::istream& operator>>(std::istream& t, Array &r)
{
    std::string str;
    t >> str;
    r = str;
    return t;
}
