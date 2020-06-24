#ifndef REFERENCE_H
#define REFERENCE_H
#include <cinttypes>

class Array;
class reference // proxy-класс для перегрузки операции индексирования
{
    friend class Array;
    // синонимы типов
    using size_type = size_t;                   // тип для размера
    using value_type = uint8_t;                 // ОСНОВНОЙ тип элементов
    // переменные
    Array &ref;                                 // ссылка на основной класс
    size_type index;                            // индекс цифры, которую надо вернуть

public:
    // конструктор
    reference(Array &d, size_type _index): ref(d), index(_index) {}
    // проксируем операции присваивания (l-value)
    reference& operator=(const value_type &b);
    reference& operator+=(const value_type &b);
    reference& operator-=(const value_type &b);
    reference& operator*=(const value_type &b);
    reference& operator/=(const value_type &b);
    reference& operator&=(const value_type &b);
    reference& operator|=(const value_type &b);
    reference& operator^=(const value_type &b);
    reference& operator%=(const value_type &b);

    reference& operator++() noexcept;
    reference operator++(int) noexcept;
    reference& operator--() noexcept;
    reference operator--(int) noexcept;
    // r-value
    operator value_type() const;
};

class const_reference // proxy-класс для перегрузки операции индексирования
{
    friend class Array;
    // синонимы типов
    using size_type = size_t;                   // тип для размера
    using value_type = uint8_t;                 // ОСНОВНОЙ тип элементов
    // переменные
    const Array &ref;                                 // ссылка на основной класс
    size_type index;                            // индекс цифры, которую надо вернуть

public:
    // конструктор
    const_reference(const Array &d, size_type _index): ref(d), index(_index) {}
    // r-value
    operator value_type() const;
};

#endif // REFERENCE_H
