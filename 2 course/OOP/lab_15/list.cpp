#include "list.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

List::Node* List::Node::headPool = nullptr;

List::reference List::front()
{
    if(_size != 0) return head->ref();
    throw std::length_error("Список пуст.");
}

List::reference List::back()
{
    if(_size != 0) return tail->ref();
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
            push_back(it->ref());
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
    newNode->ref() = N;
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
    newNode->ref() = N;
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
        newNode->ref() = b;
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
        if ( (it1->value()) != (it2->value()) ) return false;
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
    return (a.value() == b.value());
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
    return ptr->ref();
}

void *List::Node::operator new(size_t size) noexcept(false)
{
    if (size != sizeof(Node))				// не объект List
    { return ::operator new(size); }		// направляем стандартному new
    Node *address = headPool;				// возвращаемый адрес
    if (address != nullptr) headPool = address->nextInPool;
    else									// пул объектов пуст
    {
        Node *block = static_cast<Node*>(::operator new(poolSize*sizeof(Node)));
        for(int i = 1; i < poolSize-1; ++i)
            block[i].nextInPool = &block[i+1]; 		// связываем элементы пула в список
        block[poolSize-1].nextInPool = nullptr;
        headPool = &block[1];
        address = block;
    }
    return address;
}
void List::Node::operator delete(void *ListObject, size_t size)
{
    if(ListObject == nullptr) return;					// нулевой адрес
    if(size!=sizeof(Node)) 						// другой объект
    { ::operator delete(ListObject);
        return;
    }
    Node *p = static_cast<Node*>(ListObject); 	// прицепили в пул
    p->nextInPool = headPool;
    headPool = p;
}
