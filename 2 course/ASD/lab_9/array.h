#ifndef ARRAY_H
#define ARRAY_H
#include <cinttypes>
#include <initializer_list>
#include <string>
#include <tuple>
#include <iterator>
#include "btree.h"

class Array
{
public:
    const int keysInPage = 256;
    // -- синонимы типов – //
    using size_type = size_t;                   // -- тип для размера -- //
    using lodger =  std::tuple<
    uint8_t,   // Family_member_count
    uint16_t,   // Year
    uint8_t,    // Month
    float,      // Area_occupied
    uint8_t,    // Rooms_count
    float       // Area_required
    >; // -- квартирант/очередник -- //
    using key_t = uint64_t;
    using value_type = std::tuple<key_t,lodger>;
    using reference = value_type&;              // -- тип ссылка -- //
    using const_reference = const value_type&;  // -- тип константная ссылка -- //
    // -- разные проверки -- //
    inline void CheckZeroSize(size_type n) const;     // -- размер не должен быть равен 0 -- //
    value_type find(key_t key);
    // -- получить size, получить data, to_string, указатели begin и end -- //
    inline size_type size() const noexcept { return _size; }
    inline value_type* data() const noexcept { return _data; }
    inline value_type* begin() const noexcept { return _data; }
    inline value_type* end() const noexcept { return (_data+_size); }
    inline value_type* begin() noexcept { return _data; }
    inline value_type* end()  noexcept { return (_data+_size); }
    static key_t get_key(const value_type &elem) noexcept {return std::get<0>(elem);};
    static lodger get_value(const value_type &elem) noexcept {return std::get<1>(elem);};
    size_type capacity() const noexcept;
    // -- конструкторы и присваивание -- //
    Array() {tree = BTree(keysInPage);};
    explicit Array(size_type n, value_type t = value_type());
    Array(const std::initializer_list<value_type> &t);
    Array(const Array& other);
    Array(Array&& other) noexcept;              // -- конструктор переноса -- //
    Array& operator=(const Array& other);
    Array& operator=(Array&& other) noexcept;   // -- операция перемещения -- //
    virtual ~Array() { delete[] _data; }
    void displayTree();
    // -- индексирование и push/pop back -- //
    reference operator[](key_t key);
    const_reference operator[](key_t key) const;
    void push_back(value_type b);
    bool erase(key_t key);
    // -- исключения -- //
    class zero_array_size_exception {};

private:
    size_type _size = 0;   // -- количество элементов -- //
    size_type _capacity = 0;     // -- реальный размер массива -- //
    value_type *_data = nullptr;   // -- динамический массив -- //
public:
    BTree tree;
};

#endif // ARRAY_H
