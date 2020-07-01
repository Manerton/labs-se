#include "array.h"
#include "doctest.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

void Array::CheckZeroSize(size_type n) const
{
    if (!n) throw zero_array_size_exception();
}

Array::value_type Array::find(key_t key)
{
    return _data[tree.find(key)];
}

Array::Array(size_type n, value_type t)
{
    CheckZeroSize(n);
    _capacity = n;
    _data = new value_type[_capacity];
    std::fill(begin(),end(),t);
    tree = BTree(keysInPage);
}

Array::Array(const std::initializer_list<value_type> &t) : Array(t.size())   // -- делегируем конструктуру выше -- //
{
    std::copy(t.begin(),t.end(),begin());
}

Array::Array(const Array &other) : _size(other._size), _capacity(other._capacity), _data(new value_type[_capacity]()), tree(other.tree)
{
    std::copy(other.begin(),other.end(),begin());
}

Array::Array(Array &&other) noexcept : _size(other._size), _capacity(other._capacity), _data(other._data), tree(other.tree)
{
    other._size = 0;
    other._capacity = 0;
    other._data = nullptr;
}

Array &Array::operator=(const Array &other) // -- по Саттеру -- //
{
    if (this != &other)
    {
        Array temp(other);
        std::swap(_size,temp._size);
        std::swap(_data,temp._data);
        std::swap(_capacity,temp._capacity);
        std::swap(tree,temp.tree);
    }
    return *this;
}

Array &Array::operator=(Array &&other) noexcept
{
    if (this != &other)
    {
        std::swap(_size,other._size);
        std::swap(_data,other._data);
        std::swap(_capacity,other._capacity);
        std::swap(tree,other.tree);
    }
    return *this;
}

void Array::displayTree()
{
    tree.traverse();
}

Array::reference Array::operator[](key_t key)
{
    size_t index = tree.find(key);
    MESSAGE(key << " <-> " << index);
    // если с таким ключом ничего не нашлось
    if (std::get<0>(_data[index]) != key) throw std::out_of_range("С таким ключом ничего не нашлось");
    return _data[index];
}

Array::const_reference Array::operator[](key_t key) const
{
    size_t index = tree.find(key);
    MESSAGE(key << " <-> " << index);
    // если с таким ключом ничего не нашлось
    if (std::get<0>(_data[index]) != key) throw std::out_of_range("С таким ключом ничего не нашлось");
    return _data[index];
}

void Array::push_back(value_type b)
{
    key_t key = std::get<0>(b);
    if (_size < _capacity)
    {
        _data[_size] = b;
        tree.insert({key,_size});
    }
    else // если кончился наш пул
    {
        Array t(_capacity*2);   // -- создаю новый массив, у которого физический размер в 2 раза больше чем сейчас -- //
        t._size = _size;
        t.tree = tree;
        std::move(begin(),end(),t.begin()); // -- перемещаю элементы из старого массива -- //
        t._data[_size] = b;
        t.tree.insert({key,_size});
        *this = std::move(t);   // -- вызываю конструктор перемещения, меняю t и this местами, старый удаляется при выходе из этого метода -- //
    }
    ++_size;
}


bool Array::erase(key_t key)
{
    // удаляемый элемент поменяем с последним и удалим его из дерева
    // из-за того, что мы их поменяли местами изменим значения индекса в дереве
    // для того, что был на конце массива
    // уменьшим размер массива
    size_t index = tree.find(key);
    auto toDelete = _data[index];

    // элемент для удаления был найден в дереве
    if (get_key(toDelete) == key)
    {
        auto last = _data[_size-1]; // последний элемент
        tree.remove(key);
        // заменяю для нашего бывшего последнего элемента индекс
        tree.replace({get_key(last),index});
        _data[index] = last;
        --_size;
    }
    return false;
}
