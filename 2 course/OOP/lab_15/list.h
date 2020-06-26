#ifndef List_H
#define List_H
#include <cinttypes>
#include <initializer_list>
#include <string>
#include <iterator>
#include "unemployedelem.h"

// В Node у List хранится указатель на value_type (на элемент внутри узла списка)
class List
{
public:
    // -- синонимы типов –- //
    using size_type = size_t;                   // -- тип для размера -- //
    using value_type = UnemployedElem;                 // -- ОСНОВНОЙ тип элементов -- //
    using reference = value_type&;              // -- тип ссылка -- //
    using const_reference = const value_type&;  // -- тип константная ссылка -- //
    // -- узел листа -- //
    struct Node
    {
        // конструкторы
        Node() : Object(new value_type()){};
        ~Node() {delete Object;}
        // получение и установка значения для элемента
        value_type value() const {return *Object;} ;
        reference ref(){return *Object;};
        void set_value (const value_type &b)
        {
            *Object = b;
        }
        Node *next = nullptr;
        Node *prev = nullptr;
        // переопределяем new
        static void * operator new(size_t size) noexcept(false);
        static void operator delete(void *ListObject, size_t size);
        friend bool operator==(const Node &a, const Node &b);
        friend bool operator!=(const Node &a, const Node &b);
    private:
        static const int poolSize = 20;		// сколько объектов в пуле
        static Node *headPool;				// заголовок пула-списка
        union {
            Node *nextInPool;
            value_type *Object;
        };
    };
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
    };  // -- конец итератора --
    struct reverse_iterator
    {
        Node *ptr = nullptr;
        // Конструкторы
        explicit reverse_iterator(Node* t) noexcept;
        reverse_iterator() = default;
        // Сравнение итераторов
        bool operator==(const reverse_iterator &it) const noexcept;
        bool operator!=(const reverse_iterator &it) const noexcept;
        bool isNullptr() const noexcept;
        // Перемещение итератора
        reverse_iterator& operator++();             // вперед
        reverse_iterator& operator--();             // назад
        reference operator*();                      // разыменование
    };  // -- конец итератора --
    // -- размеры -- //
    inline size_type size() const noexcept { return _size; }
    bool empty() const noexcept { return _size == 0; };
    // -- итераторы --
    iterator begin() noexcept { return iterator(head); };
    iterator end() noexcept { return iterator(tail); };
    iterator begin() const noexcept  { return iterator(head); };
    iterator end() const noexcept  { return iterator(head); };
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
