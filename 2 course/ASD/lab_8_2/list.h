#ifndef List_H
#define List_H
#include <cinttypes>
#include <initializer_list>
#include <string>
#include <iterator>

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
        iterator operator++(int);              // вперед
        iterator operator--(int);              // назад
        reference operator*();               // разыменование
    };
    // -- размеры -- //
    inline size_type size() const noexcept { return _size; }
    bool empty() const noexcept { return _size == 0; };
    // -- итераторы --
    iterator<T> begin() noexcept { return iterator<T>(head); };
    iterator<T> end() noexcept { return iterator<T>(end_node); };
    iterator<T> begin() const noexcept { return iterator<T>(head); };
    iterator<T> end() const noexcept { return iterator<T>(end_node); };
    // -- доступ к элементам --
    reference front();
    reference back();
    // -- конструкторы и присваивание -- //
    List();
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
    void unique() noexcept;
    // сравнения
    template <typename Y> friend bool operator== (const List<Y> &a, const List<Y> &b);
    template <typename Y> friend bool operator!=(const List<Y> &a, const List<Y> &b);
private:
    size_type _size = 0; // -- количество элементов -- //
    Node *head = nullptr;   // первый элемент
    Node *tail = nullptr;   // последний элемент
    Node *end_node = nullptr;   // фиктивный элемент идущий за последним
};

#include "list.inl" // подключаем реализацию

#endif // List_H

