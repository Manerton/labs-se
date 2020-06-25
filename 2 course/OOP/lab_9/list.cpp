#include "list.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

//std::string List::toString() const noexcept
//{
//    std::stringstream ss;
//    for(iterator i = head; i != nullptr; i = i->next) ss << i->N << " ";
//    std::string s = ss.str();
//    s.pop_back(); // убираем лишний пробел
//    return s;
//}

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
    if (it->prev == nullptr) { push_front(b); }
    else if (it->next == nullptr) {  push_back(b); }
    else
    {
        Node* newNode = new Node;
        newNode->N = b;
        newNode->prev = it->prev;
        newNode->next = it;
        newNode->prev->next = newNode;
        it->prev = newNode;
    }
    return it;
}

List::iterator List::erase(List::iterator it)
{
    if (!empty())
    {
        if (it == head) { pop_front(); }
        else if (it == tail) { pop_back(); }
        else
        {
            iterator toDelete = it;
            it->prev->next = it->next;
            it->next->prev = it->prev;
            it = it->next;
            delete toDelete;
            _size--;
        }
    }
    return it;
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

