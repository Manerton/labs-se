#include "list.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

const_iterator List::begin() const noexcept
{
    return(const_iterator(head));
}

const_iterator List::end() const noexcept
{
    return(const_iterator(tail));
}

const_reverse_iterator List::rbegin() const noexcept
{
    return(const_reverse_iterator(tail));
}

const_reverse_iterator List::rend() const noexcept
{
    return(const_reverse_iterator(head));
}

List::reference List::front()
{
    if(_size != 0) return head->N;
    throw std::length_error("Список пуст.");
}

List::reference List::back()
{
    if(_size != 0) return tail->N;
    throw std::length_error("Список пуст.");
}

List::List(List::size_type n, List::const_reference v)
{
    if (n != 0)
    {
        for (size_type i = 0; i < n; ++i)
        {
            push_back(v);
        }
    }
}

List::List(const std::initializer_list<List::value_type> &t)
{
    if (t.size() != 0)
    {
        for (auto& v : t)
        {
            push_back(v);
        }
    }
}

List::List(const List &other)
{
    if (!other.empty())
    {
        for(auto it = other.head; it != nullptr; it = it->next)
        {
            push_back(it->N);
        }
    }
}

List::List(List &&other) noexcept : _size(other.size()), head(other.head), tail(other.tail)
{
    other._size = 0;
    other.head = nullptr;
    other.tail = nullptr;
}

List &List::operator=(const List &other) // -- по Саттеру -- //
{
    if (this != &other)
    {
        List temp(other);               // копируем из other новое содержимое
        std::swap(_size,temp._size);    // свапаем так, чтобы копия оказалась нашим новым списком
        std::swap(head,temp.head);      // а наш старый список теперь temp
        std::swap(tail,temp.tail);
    }                                   // temp удалится вызовом деструктора
    // после выхода из этого if
    return *this;
}

List &List::operator=(List &&other) noexcept
{
    if (this != &other)
    {
        std::swap(_size,other._size);
        std::swap(head,other.head);
        std::swap(tail,other.tail);
    }
    return *this;
}

List::~List()
{
    clear();
}

void List::push_front(List::const_reference N)
{
    Node* newNode = new Node;
    newNode->N = N;
    newNode->next = nullptr;
    newNode->prev = nullptr;
    if (empty()) // если очередь пустая
    {
        head = newNode;
        tail = head;
    }
    else
    {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    _size++;
}

void List::pop_front()
{
    if(!empty())
    {
        Node* toDelete = head;
        if (head == tail)   // в очереди только один узел
        {
            head = nullptr;
            tail = nullptr;
        }
        else
        {
            head = head->next;
            head->prev = nullptr;
        }
        delete toDelete;
        _size--;
    }
}

void List::push_back(List::const_reference N)
{
    Node* newNode = new Node;
    newNode->N = N;
    newNode->next = nullptr;
    newNode->prev = nullptr;
    if (empty()) // если очередь пустая
    {
        head = newNode;
        tail = head;
    }
    else
    {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    _size++;
}

void List::pop_back()
{
    if(!empty())
    {
        Node* toDelete = tail;
        if (head == tail)   // в очереди только один узел
        {
            head = nullptr;
            tail = nullptr;
        }
        else
        {
            tail = tail->prev;
            tail->next = nullptr;
        }
        delete toDelete;
        _size--;
    }
}

List::iterator List::insert(List::iterator it, List::const_reference b)
{
    auto ptr = it.ptr;
    if (ptr->prev == nullptr) { push_front(b); }
    else if (ptr->next == nullptr) {  push_back(b); }
    else
    {
        Node* newNode = new Node;
        newNode->N = b;
        newNode->prev = ptr->prev;
        newNode->next = ptr;
        newNode->prev->next = newNode;
        ptr->prev = newNode;
    }
    return iterator(ptr);
}

List::iterator List::erase(List::iterator it)
{
    auto ptr = it.ptr;
    if (!empty())
    {
        if (ptr == head) { pop_front(); }
        else if (ptr == tail) { pop_back(); }
        else
        {
            auto toDelete = ptr;
            ptr->prev->next = ptr->next;
            ptr->next->prev = ptr->prev;
            ptr = ptr->next;
            delete toDelete;
            _size--;
        }
    }
    return iterator(ptr);
}

void List::clear()
{
    while (!empty()) pop_front();
}

void List::swap(List &t) noexcept
{
    if (head != t.head)
    {
        std::swap(_size,t._size);
        std::swap(head,t.head);
        std::swap(tail,t.tail);
    }
}

// операции сравнения для списка
bool operator==(const List &a, const List &b)
{
    if (a.size() != b.size()) return false;
    for(auto it1 = a.head, it2 = b.head; it1 != nullptr; it1 = it1->next, it2 = it2->next)
    {
        if ( (it1->N) != (it2->N) ) return false;
    }
    return true;
}

bool operator!=(const List &a, const List &b)
{
    return !(a == b);
}

// для узлов
bool operator==(const List::Node &a, const List::Node &b)
{
    return (a.N == b.N);
}

bool operator!=(const List::Node &a, const List::Node &b)
{
    return !(a == b);
}

// вложенный итератор
List::iterator::iterator(List::Node *t) noexcept
{
    ptr = t;
}

bool List::iterator::operator==(const List::iterator &it) const noexcept
{
    return (ptr == it.ptr);
}

bool List::iterator::operator!=(const List::iterator &it) const noexcept
{
    return !(*this == it);
}

bool List::iterator::isNullptr() const noexcept
{
    return (ptr == nullptr);
}

List::iterator &List::iterator::operator++()
{
    ptr = ptr->next;
    return *this;
}

List::iterator &List::iterator::operator--()
{
    ptr = ptr->prev;
    return *this;
}

List::reference List::iterator::operator*()
{
    return ptr->N;
}
// внешний итератор (для чтения)
const_iterator::const_iterator(List::Node *t) noexcept
{
    ptr = t;
}

bool const_iterator::operator==(const const_iterator &it) const noexcept
{
    return (ptr == it.ptr);
}

bool const_iterator::operator!=(const const_iterator &it) const noexcept
{
    return !(*this == it);
}

const_iterator &const_iterator::operator++()
{
    ptr = ptr->next;
    return *this;
}

const_iterator &const_iterator::operator--()
{
    ptr = ptr->prev;
    return *this;
}

List::const_reference const_iterator::operator*()
{
    return ptr->N;
}

bool const_iterator::isNullptr() const noexcept
{
    return (ptr == nullptr);
}
// вложенный обратный итератор
List::reverse_iterator::reverse_iterator(List::Node *t) noexcept
{
    ptr = t;
}

bool List::reverse_iterator::operator==(const List::reverse_iterator &it) const noexcept
{
    return (ptr == it.ptr);
}

bool List::reverse_iterator::operator!=(const List::reverse_iterator&it) const noexcept
{
    return !(*this == it);
}

bool List::reverse_iterator::isNullptr() const noexcept
{
    return (ptr == nullptr);
}

List::reverse_iterator &List::reverse_iterator::operator++()
{
    ptr = ptr->prev;
    return *this;
}

List::reverse_iterator &List::reverse_iterator::operator--()
{
    ptr = ptr->next;
    return *this;
}

List::reference List::reverse_iterator::operator*()
{
    return ptr->N;
}

// внешний константый обратный итератор
// внешний итератор (для чтения)
const_reverse_iterator::const_reverse_iterator(List::Node *t) noexcept
{
    ptr = t;
}

bool const_reverse_iterator::operator==(const const_reverse_iterator &it) const noexcept
{
    return (ptr == it.ptr);
}

bool const_reverse_iterator::operator!=(const const_reverse_iterator &it) const noexcept
{
    return !(*this == it);
}

const_reverse_iterator &const_reverse_iterator::operator++()
{
    ptr = ptr->prev;
    return *this;
}

const_reverse_iterator &const_reverse_iterator::operator--()
{
    ptr = ptr->next;
    return *this;
}

List::const_reference const_reverse_iterator::operator*()
{
    return ptr->N;
}

bool const_reverse_iterator::isNullptr() const noexcept
{
    return (ptr == nullptr);
}
