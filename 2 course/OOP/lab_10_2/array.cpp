#include "array.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

void Array::CheckZeroSize(Array::size_type n) const
{
    if (!n) throw std::length_error{"bad index exception"};
}

void Array::CheckIndex(size_type index) const
{
    if (index >= _size) throw std::out_of_range{"bad index exception"};
}

const_iterator Array::begin() const noexcept
{
    return const_iterator(data);
}

const_iterator Array::end() const noexcept
{
    return const_iterator(data+_size);
}

Array::Array(size_type n, const_reference t)
{
    CheckZeroSize(n);
    _size = n;
    data = new value_type[_size];
    std::fill(begin(),end(),t);
}

Array::Array(const std::initializer_list<value_type> &t) : Array(t.size())   // -- делегируем конструктуру выше -- //
{
    std::copy(t.begin(),t.end(),begin());
}

Array::Array(const Array &other) : _size(other._size), data(new value_type[_size])
{
    std::copy(other.begin(),other.end(),begin());
}

Array::Array(Array &&other) noexcept : _size(other._size), data(other.data)
{
    other._size = 0;
    other.data = nullptr;
}

Array &Array::operator=(const Array &other) // -- по Саттеру -- //
{
    if (this != &other)
    {
        Array temp(other);
        std::swap(_size,temp._size);
        std::swap(data,temp.data);
    }
    return *this;
}

Array &Array::operator=(Array &&other) noexcept
{
    if (this != &other)
    {
        std::swap(_size,other._size);
        std::swap(data,other.data);
    }
    return *this;
}

Array::reference Array::front()
{
    if(!empty()) return *(begin());
    throw std::length_error("Массив пуст.");
}

Array::const_reference Array::front() const
{
    if(!empty()) return *(begin());
    throw std::length_error("Массив пуст.");
}

Array::reference Array::back()
{
    if(!empty()) return *(--end());
    throw std::length_error("Массив пуст.");
}

Array::const_reference Array::back() const
{
    if(!empty()) return *(--end());
    throw std::length_error("Массив пуст.");
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

void Array::push_front(Array::const_reference b)
{
    Array t(_size+1, b);   // -- создаю новый массив, у которого размер больше на 1 элемент -- //
    std::move(begin(),end(),++t.begin()); // -- перемещаю элементы из старого массива -- //
    *this = std::move(t);   // -- вызываю конструктор перемещения, меняю t и this местами, старый удаляется при выходе из этого метода -- //
}

void Array::pop_front()
{
    Array t(_size-1);   // -- создаю новый массив, у которого размер больше на 1 элемент -- //
    auto it = ++begin();
    std::move(it,end(),t.begin()); // -- перемещаю элементы из старого массива -- //
    *this = std::move(t);   // -- вызываю конструктор перемещения, меняю t и this местами, старый удаляется при выходе из этого метода -- //
}

void Array::push_back(const_reference b)
{
    Array t(_size+1, b);   // -- создаю новый массив, у которого размер больше на 1 элемент -- //
    std::move(begin(),end(),t.begin()); // -- перемещаю элементы из старого массива -- //
    *this = std::move(t);   // -- вызываю конструктор перемещения, меняю t и this местами, старый удаляется при выходе из этого метода -- //
}

void Array::pop_back()
{
    Array t(_size-1);   // -- создаю новый массив, у которого размер меньше на 1 элемент -- //
    auto it = --end();
    std::move(begin(),it,t.begin()); // -- перемещаю элементы из старого массива -- //
    *this = std::move(t);   // -- вызываю конструктор перемещения, меняю t и this местами, старый удаляется при выходе из этого метода -- //
}

Array::iterator Array::insert(Array::iterator it, Array::const_reference b)
{
    if (it == begin()) { push_front(b); }
    else if (it == end()) {  push_back(b); }
    else
    {
        Array t(_size+1,b);

        size_type dist = size_type(it.ptr - begin().ptr);    // расстояние между первым элементом и указанным итератором
        std::move(begin(),it,t.begin());                // -- перемещаю элементы из старого массива до указанного итератора -- //
        auto new_it = iterator(t.begin().ptr + dist + 1);          // итератор на тот же элемент, перед которым мы добавляем новый элемент, в новом массиве t
        std::move(it,end(),new_it);                   // перемещаем элементы справа от итератора (после итератора)
        *this = std::move(t);
        it = new_it;
    }
    return it;
}

Array::iterator Array::insert(Array::size_type i, Array::const_reference b)
{
    auto it = iterator(data+i);
    if (i == 0) { push_front(b); }
    else if (i == _size-1) {  push_back(b); }
    else
    {
        Array t(_size+1,b);
        std::move(begin(),it,t.begin());                // -- перемещаю элементы из старого массива до указанного итератора -- //
        auto new_it = iterator(t.begin().ptr + i + 1);
        std::move(it,end(),new_it);                   // перемещаем элементы справа от итератора (после итератора)
        *this = std::move(t);
        it = new_it;
    }
    return it;
}

Array::iterator Array::erase(Array::iterator it)
{
    if (it == begin()) { pop_front(); }
    else if (it == end()) {  pop_back(); }
    else
    {
        Array t(_size-1);

        size_type dist = size_type(it.ptr - begin().ptr);    // расстояние между первым элементом и указанным итератором
        std::move(begin(),it,t.begin());                // -- перемещаю элементы из старого массива до указанного итератора -- //
        auto new_it = iterator(t.begin().ptr + dist);          // итератор на тот же элемент, перед которым мы добавляем новый элемент, в новом массиве t
        std::move(++it,end(),new_it);                   // перемещаем элементы справа от итератора (после итератора)
        *this = std::move(t);
        it = new_it;
    }
    return it;
}

Array::iterator Array::erase(Array::size_type i)
{
    auto it = iterator(data+i);
    if (i == 0) { pop_front(); }
    else if (i == _size-1) {  pop_back(); }
    else
    {
        Array t(_size-1);
        std::move(begin(),it,t.begin());                // -- перемещаю элементы из старого массива до указанного итератора -- //
        auto new_it = iterator(t.begin().ptr + i);
        std::move(++it,end(),new_it);                   // перемещаем элементы справа от итератора (после итератора)
        *this = std::move(t);
        it = new_it;
    }
    return it;
}

bool operator==(const Array &a, const Array &b)
{
    if (a._size != b._size) return false;
    return std::equal(a.begin(),a.end(),b.begin());
}

bool operator!=(const Array &a, const Array &b)
{
    return !(a == b);
}

// вложенный итератор
Array::iterator::iterator(Array::value_type *t) noexcept
{
    ptr = t;
}

bool Array::iterator::operator==(const Array::iterator &it) const noexcept
{
    return (ptr == it.ptr);
}

bool Array::iterator::operator!=(const Array::iterator &it) const noexcept
{
    return !(*this == it);
}

Array::iterator &Array::iterator::operator++()
{
    ++ptr;
    return *this;
}

Array::iterator &Array::iterator::operator--()
{
    --ptr;
    return *this;
}

Array::reference Array::iterator::operator*()
{
    return *ptr;
}

bool Array::iterator::isNullptr() const noexcept
{
    return (ptr == nullptr);
}

// внешний итератор (для чтения)
const_iterator::const_iterator(Array::value_type *t) noexcept
{
    ptr = t;
}

bool const_iterator::operator==(const const_iterator &it) const noexcept
{
    return (ptr == it.ptr);
}

bool const_iterator::operator!=(const const_iterator &it) const noexcept
{
    return !(*this == it);
}

const_iterator &const_iterator::operator++()
{
    ++ptr;
    return *this;
}

const_iterator &const_iterator::operator--()
{
    --ptr;
    return *this;
}

Array::const_reference const_iterator::operator*()
{
    return *ptr;
}

bool const_iterator::isNullptr() const noexcept
{
    return (ptr == nullptr);
}
