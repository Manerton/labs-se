#ifndef ARRAY_H
#define ARRAY_H
#include <cinttypes>
#include <initializer_list>
#include <string>
#include <iterator>
#include "reference.h"

class Array
{
public:
    // -- синонимы типов – //
    using size_type = size_t;                   // -- тип для размера -- //
    using value_type = uint8_t;                 // -- ОСНОВНОЙ тип элементов -- //
    friend class reference;
    friend class const_reference;
    // -- разные проверки -- //
    inline void CheckZeroSize(size_type n) const;     // -- размер не должен быть равен 0 -- //
    inline void CheckIndex(size_type index) const;     // -- проверка индекса -- //

    // -- получить size, получить data, to_string, указатели begin и end -- //
    inline size_type get_size() const noexcept { return size_array; }
    inline value_type* get_data() const noexcept { return data; }
    std::string toString() const noexcept;
    inline value_type* begin() const noexcept { return data; }
    inline value_type* end() const noexcept { return (data+size_array); }

    // -- конструкторы и присваивание -- //
    Array() = default;
    explicit Array(size_type n, value_type t = 0);
    Array(const std::initializer_list<value_type> &t);
    Array(const std::string &str);
    Array(const Array& other);
    Array(Array&& other) noexcept;              // -- конструктор переноса -- //
    Array& operator=(const Array& other);
    Array& operator=(Array&& other) noexcept;   // -- операция перемещения -- //
    virtual ~Array() { delete data; }

    // -- индексирование и push/pop back -- //
    reference operator[](size_type index);
    const_reference operator[](size_type index) const;
    void push_back(value_type b);
    void pop_back();
    value_type* erase(value_type* first, value_type* last);
    // -- немного сравнений на равенство -- //
    friend bool operator==(const Array &a, const Array &b);
    friend bool operator!=(const Array &a, const Array &b);
    friend bool operator<(const Array &a, const Array &b);
    friend bool operator>=(const Array &a, const Array &b);
    friend bool operator>(const Array &a, const Array &b);
    friend bool operator<=(const Array &a, const Array &b);
    // -- дружественные функции ввода/вывода -- //
    friend std::ostream& operator<<(std::ostream& t, const Array &r);
    friend std::istream& operator>>(std::istream& t, Array &r);
    // -- исключения -- //
    class zero_array_size_exception {};

private:
    size_type size_array = 0; // -- количество элементов -- //
    value_type *data = nullptr;   // -- динамический массив -- //

};

#endif // ARRAY_H
