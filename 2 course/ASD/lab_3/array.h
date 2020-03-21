#ifndef ARRAY_H
#define ARRAY_H
#include <cinttypes>
#include <initializer_list>
#include <string>

class Array
{
public:
    // -- синонимы типов – //
    using size_type = size_t;                   // -- тип для размера -- //
    using value_type = uint8_t;                 // -- ОСНОВНОЙ тип элементов -- //
    using iterator = value_type*;               // -- это просто указатель -- //
    using const_iterator = const value_type*;   // -- указатель на константу -- //
    using reference = value_type&;              // -- тип ссылка -- //
    using const_reference = const value_type&;  // -- тип константная ссылка -- //

    // -- разные проверки для исключений -- //
    inline void CheckZeroSize(size_type n) const;     // -- размер не должен быть равен 0 -- //
    inline void CheckIndex(size_type index) const;     // -- проверка индекса -- //

    // -- конструкторы и присваивание -- //
    Array() = default;
    explicit Array(size_type n, value_type t = 0);
    Array(const std::initializer_list<value_type> &t);
    ///Array(const std::string &str);
    Array(const Array& other);
    Array(Array&& other) noexcept;              // -- конструктор переноса -- //
    Array& operator=(const Array& other);
    Array& operator=(Array&& other) noexcept;   // -- операция перемещения -- //
    inline ~Array() { delete data; }

    // -- размеры -- //
    inline size_type size() const noexcept { return size_array; }   // -- текущее кол-во элементов -- //
    inline bool empty() const noexcept { return size_array == 0; }  // -- есть ли элементы -- //
    inline size_type capacity() const noexcept { return capacity_array; } // -- потенциальный размер -- //

    // -- операции с итераторами/указателями -- //
    ///std::string toString() const noexcept;
    inline iterator begin() noexcept { return data; }
    inline const_iterator cbegin() const noexcept { return data; }
    inline iterator end() noexcept { return (data+size_array); }
    inline const_iterator cend() const noexcept { return (data+size_array); }

    // -- индексирование и доступ к элементам -- //
    reference operator[](size_type index);
    const_reference operator[](size_type index) const;
    reference front() noexcept;
    const_reference front() const noexcept;
    reference back() noexcept;
    const_reference back() const noexcept;

    // -- методы-модификаторы -- //
    void push_back(const value_type& b);
    void pop_back() noexcept;
    void push_front(const value_type& b);
    void pop_front() noexcept;

    iterator insert(const_iterator position, const value_type& x);
    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);

    void clear() noexcept;  // -- очистить массив -- //
    void swap(Array& rhs);  // -- обмен массивов -- //

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
    size_type capacity_array = 0; // -- размер дин. массива -- //
    value_type *data = nullptr;   // -- динамический массив -- //

};

#endif // ARRAY_H
