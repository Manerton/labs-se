#ifndef List_H
#define List_H
#include <cinttypes>
#include <initializer_list>
#include <string>
#include <iterator>
#include "unemployedelem.h"

class List
{
public:
// -- синонимы типов –- //
    using size_type = size_t;                   // -- тип для размера -- //
    using value_type = UnemployedElem;                 // -- ОСНОВНОЙ тип элементов -- //
    using reference = value_type&;              // -- тип ссылка -- //
    using const_reference = const value_type&;  // -- тип константная ссылка -- //

    struct Node;
    using iterator = Node*;
// -- узел листа -- //
    struct Node
    {
        value_type N;
        Node *next = nullptr;
        Node *prev = nullptr;

        friend bool operator==(const Node &a, const Node &b);
        friend bool operator!=(const Node &a, const Node &b);
    };
// -- toString -- //
    //std::string toString() const noexcept;
// -- размеры -- //
    inline size_type size() const noexcept { return _size; }
    bool empty() const noexcept { return _size == 0; };
// -- итераторы --
    iterator begin() noexcept { return head; };
    iterator end() noexcept { return tail; };
    iterator begin() const noexcept { return head; }
    iterator end() const noexcept { return tail; }
// -- доступ к элементам --
    reference front();
    reference back();
// -- конструкторы и присваивание -- //
    List() = default;
    explicit List(size_type n, const_reference = value_type());
    List(const std::initializer_list<value_type> &t);
    List(const List& other);
    List(List&& other) noexcept;              // -- конструктор переноса -- //
    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;   // -- операция перемещения -- //
    virtual ~List();
    // -- модификаторы контейнера -- //
    void push_front(const_reference);
    void pop_front();
    void push_back(const_reference);
    void pop_back();
    iterator insert(iterator, const_reference);
    iterator erase(iterator );
    void clear();                   // удалить все
    void swap(List &t) noexcept;    // обменять с заданным списком
    // сравнения
    friend bool operator==(const List &a, const List &b);
    friend bool operator!=(const List &a, const List &b);
private:
    size_type _size = 0; // -- количество элементов -- //
    Node *head = nullptr;
    Node *tail = nullptr;
};

#endif // List_H
