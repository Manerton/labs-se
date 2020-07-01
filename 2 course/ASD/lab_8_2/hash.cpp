#include "hash.h"
#include "doctest.h"
#include <stack>
#include <vector>
#include <algorithm>
#include <sstream>

std::string elemToString(const hash::data_t &element)
{
    std::stringstream ss;
    ss << hash::get_key(element) << " "
       << int(std::get<0>(hash::get_value(element))) << " "
       << std::get<1>(hash::get_value(element)) << " "
       << int(std::get<2>(hash::get_value(element))) << " "
       << std::get<3>(hash::get_value(element)) << " "
       << int(std::get<4>(hash::get_value(element))) << " "
       << std::get<5>(hash::get_value(element));
    return ss.str();
}

size_t hash::get_gorner_result(uint64_t key, uint16_t x)
{
    std::vector<uint64_t> numbers;

    while (key != 0)
    {
        numbers.push_back(key % 10);
        key /= 10;
    }
    std::reverse(numbers.begin(), numbers.end());
    for (size_t i = 1; i < numbers.size(); ++i)
    {
        numbers[i] += numbers[i - 1] * x;
    }
    return numbers.back();
}

auto hash::find_iterator(const hash::key_t &key)
{
    size_t index = callCurrentHashFunc(key);
    if (arr != nullptr && !arr[index].chain.empty())
    {
        // если по вычисленному индексу не тот элемент
        if (arr[index].key() != key)
        {
            for (auto it = arr[index].chain.begin(); !it.isNullptr(); ++it)
            {
                if (get_key(*it) == key) return it;
            }
        }
        else return arr[index].chain.begin(); // если нет коллизии
    }
    return (arr[index].chain.end()); // Если не удалось найти
}

// подсчет значения хэш функции как остаток от деления ключа на размер таблицы
size_t hash::calculateDivideHashFunc(const key_t& key)
{
    return key % table_size;
}

// подсчет значения хэш функции по горнеру и деление на размер таблицы
size_t hash::calculateGornerDivideHashFunc(const key_t& key)
{
    return calculateDivideHashFunc(get_gorner_result(key,x_gorner_1));
}

// подсчет значения хэш функции по горнеру и выделению цифр
size_t hash::calculateGornerHighlightHashFunc(const key_t& key)
{
    return (get_gorner_result(key,x_gorner_2) & mask);
}

hash::~hash()
{
    clear();
    delete [] arr;
    arr = nullptr;
    _count = 0;
}

hash& hash::operator=(const hash &other)
{
    if (this != &other)
    {
        hash temp(other);
        std::swap(_count,temp._count);
        std::swap(arr,temp.arr);
    }
    return *this;
}

hash::key_t hash::get_key(const hash::data_t &elem) noexcept
{
    return std::get<0>(elem);
}

hash::value_t hash::get_value(const hash::data_t &elem) noexcept
{
    return std::get<1>(elem);
}

hash::hash(const hash& other) : _count(other._count), arr(new node_t[table_size])
{
    std::copy(other.begin(),other.end(),begin());
}

hash::hash() : arr(new node_t[table_size]), currentHashFunc(divideHash)
{

}

hash::hash(const std::initializer_list<data_t> &t) : hash()
{
    std::copy(t.begin(),t.end(),begin());
}

hash::hash(const data_t& d) : hash()
{
    insert(d);
}

bool hash::empty() const noexcept
{
    return (arr == nullptr);
}

size_t hash::count() const noexcept
{
    return _count;
}

size_t hash::callCurrentHashFunc(const hash::key_t &key)
{
    if (currentHashFunc == divideHash)
    {
        return calculateDivideHashFunc(key);
    }
    else if (currentHashFunc == gornerDivide)
    {
        return calculateGornerDivideHashFunc(key);
    }
    else if (currentHashFunc == gornerHighlight)
    {
        return calculateGornerHighlightHashFunc(key);
    }
    return 0;
}

hash::hash(const hash::hashFunc _hashFunc) : hash()
{
    currentHashFunc = _hashFunc;
}

hash::data_t hash::find(const key_t& key)
{
    auto result = find_iterator(key);
    size_t index = callCurrentHashFunc(key);
    if (result != arr[index].chain.end()) return {*result};
    else return {EMPTYELEM,value_t()};
}

bool hash::insert(const key_t& key, const value_t& v)
{
    // можем получить индекс как хеш функция = остаток от деления на размер таблицы
    // гарнер x = 23 деление на размер таблицы
    // гарнер x = 256 выделение 8 или 10 бит (от размера таблицы)
    size_t index = callCurrentHashFunc(key);
    arr[index].chain.push_back({key,v});
    _count++;
    return true;
}

bool hash::insert(const data_t& d)
{
    auto [key, v] = d;
    return (insert(key,v));
}

bool hash::replace(const key_t& key, const value_t& v)
{
    auto toReplace = find_iterator(key);
    if (!toReplace.isNullptr())
    {
        *toReplace = {key,v};
        return true;
    }
    return false;
}

bool hash::erase(const key_t& key)
{
    size_t index = callCurrentHashFunc(key);
    if (!arr[index].chain.empty())
    {
        auto it = find_iterator(key);
        arr[index].chain.erase(it);
        _count--;
        return true;
    }
    return false;
}


void hash::clear()
{
    for (size_t i = 0; i < table_size; ++i)
    {
        arr[i].chain.clear();
    }
}

void hash::swap(hash& t) noexcept
{
    std::swap(arr, t.arr);
    std::swap(_count, t._count);
}
