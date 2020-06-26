#ifndef List_H
#define List_H
#include <cinttypes>
#include <initializer_list>
#include <string>
#include <iterator>

template <class Y>
class const_iterator;
template <class T> // это шаблон класса с T вместо фактического (передаваемого) типа данных
class List
{
public:
    // -- синонимы типов –- //
    using size_type = size_t;                   // -- тип для размера -- //
    using value_type = T;                 // -- ОСНОВНОЙ тип элементов -- //
    using reference = value_type&;              // -- тип ссылка -- //
    using const_reference = const value_type&;  // -- тип константная ссылка -- //
    struct Node
    {
        Node *next = nullptr;
        Node *prev = nullptr;
        value_type N;

        friend bool operator==(const Node &a, const Node &b);
        friend bool operator!=(const Node &a, const Node &b);
    };
    template <class Y>
    struct iterator
    {
        Node *ptr = nullptr;
        // Конструкторы
        explicit iterator(Node* t) noexcept;
        iterator() = default;
        // Сравнение итераторов
        bool operator==(const iterator &it) const noexcept;
        bool operator!=(const iterator &it) const noexcept;
        bool isNullptr() const noexcept;
        // Перемещение итератора
        iterator& operator++();              // вперед
        iterator& operator--();              // назад
        reference operator*();               // разыменование
    };
    // -- размеры -- //
    inline size_type size() const noexcept { return _size; }
    bool empty() const noexcept { return _size == 0; };
    // -- итераторы --
    iterator<T> begin() noexcept { return iterator<T>(head); };
    iterator<T> end() noexcept { return iterator<T>(tail); };
    const_iterator<T> begin() const noexcept;
    const_iterator<T> end() const noexcept;
    // -- доступ к элементам --
    reference front();
    reference back();
    // -- конструкторы и присваивание -- //
    List() = default;
    explicit List(size_type n, const_reference = value_type());
    List(const std::initializer_list<value_type> &t);
    List(const List<T> &other);
    List(List<T>&& other) noexcept;              // -- конструктор переноса -- //
    List& operator=(const List<T>& other);
    List& operator=(List<T>&& other) noexcept;   // -- операция перемещения -- //
    virtual ~List();
    // -- модификаторы контейнера -- //
    void push_front(const_reference);
    void pop_front();
    void push_back(const_reference);
    void pop_back();
    iterator<T> insert(iterator<T>, const_reference);
    iterator<T> erase(iterator<T>);
    void clear();                   // удалить все
    void swap(List<T> &t) noexcept;    // обменять с заданным списком
    // сравнения
    template <typename Y> friend bool operator== (const List<Y> &a, const List<Y> &b);
    template <typename Y> friend bool operator!=(const List<Y> &a, const List<Y> &b);
private:
    size_type _size = 0; // -- количество элементов -- //
    Node *head = nullptr;
    Node *tail = nullptr;
};

template <class Y>
class const_iterator
{
    using Node = typename List<Y>::Node;
    using const_reference = typename List<Y>::const_reference;
    Node *ptr = nullptr;
public:
    // Конструкторы
    explicit const_iterator(Node* t) noexcept;
    const_iterator() = default;
    bool operator==(const const_iterator &it) const noexcept;
    bool operator!=(const const_iterator &it) const noexcept;
    // Перемещение итератора
    const_iterator& operator++();              // вперед
    const_iterator& operator--();              // назад
    const_reference operator*();               // разыменование
    bool isNullptr() const noexcept;

};  // -- конец итератора --

#include "list.inl" // подключаем реализацию

#endif // List_H

