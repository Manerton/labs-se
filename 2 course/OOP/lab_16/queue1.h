#ifndef QUEUE1_H
#define QUEUE1_H
#include "array.h"

// Используя класс-шаблон Array реализовать Очередь на основе паттерна Адаптер
// через композицию
template <class T>
class Queue1
{
public:
    using size_type = size_t;                   // -- тип для размера -- //
    using value_type = T;                 // -- ОСНОВНОЙ тип элементов -- //
    using reference = value_type&;              // -- тип ссылка -- //
    using const_reference = const value_type&;  // -- тип константная ссылка -- //
    // конструкторы
    Queue1();
    ~Queue1();
    Queue1(const Queue1& other);
    Queue1(Queue1&& other) noexcept;              // -- конструктор переноса -- //
    Queue1& operator=(const Queue1& other);
    Queue1& operator=(Queue1&& other) noexcept;   // -- операция перемещения -- //
    // методы очереди
    bool empty() const noexcept;
    size_type size() const noexcept;
    reference front();
    reference back();
    void push(const_reference);
    void pop();
    // сравнение на равенство
    template <typename Y> friend bool operator==(const Queue1<Y> &a, const Queue1<Y> &b);
    template <typename Y> friend bool operator!=(const Queue1<Y> &a, const Queue1<Y> &b);
private:
    Array<T>* ptr;
};

// реализация

template <typename T>
Queue1<T>::Queue1() : ptr(new Array<T>) {}

template <typename T>
Queue1<T>::~Queue1()
{
    delete ptr;
}
template <typename T>
Queue1<T>::Queue1(const Queue1 &other) : ptr (new Array<T>(*other.ptr)) {}
template <typename T>
Queue1<T>::Queue1(Queue1 &&other) noexcept : ptr (new Array<T>(std::move(*other.ptr))) {}
template <typename T>
Queue1<T> &Queue1<T>::operator=(const Queue1<T> &other)
{
    ptr->operator=(*other.ptr);
    return *this;
}
template <typename T>
Queue1<T> &Queue1<T>::operator=(Queue1<T> &&other) noexcept
{
    ptr->operator=(std::move(*other.ptr));
    return *this;
}
template <typename T>
bool Queue1<T>::empty() const noexcept
{
    bool res = ptr->empty();
    return res;
}

template<class T>
typename Queue1<T>::size_type Queue1<T>::size() const noexcept
{
    return ptr->size();
}

template<class T>
typename Queue1<T>::reference Queue1<T>::front()
{
    return ptr->front();
}

template<class T>
typename Queue1<T>::reference Queue1<T>::back()
{
    return ptr->back();
}

template<class T>
void Queue1<T>::push(Queue1::const_reference b)
{
    ptr->push_back(b);
}

template<class T>
void Queue1<T>::pop()
{
    ptr->pop_front();
}

template<class T>
bool operator==(const Queue1<T> &a, const Queue1<T> &b)
{
    return *(a.ptr) == *(b.ptr);
}

template<class T>
bool operator!=(const Queue1<T> &a, const Queue1<T> &b)
{
    return !(a == b);
}


#endif // QUEUE1_H
