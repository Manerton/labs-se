#ifndef QUEUE2_H
#define QUEUE2_H

#include "array.h"

// Используя класс-шаблон Array реализовать Очередь на основе паттерна Адаптер
// через композицию
template <class T>
class Queue2 : private Array<T>
{
public:
    using size_type = size_t;                   // -- тип для размера -- //
    using value_type = T;                 // -- ОСНОВНОЙ тип элементов -- //
    using reference = value_type&;              // -- тип ссылка -- //
    using const_reference = const value_type&;  // -- тип константная ссылка -- //
    // конструкторы
    Queue2();
    ~Queue2() = default;
    Queue2(const Queue2& other);
    Queue2(Queue2&& other) noexcept;              // -- конструктор переноса -- //
    Queue2& operator=(const Queue2& other) = default;
    Queue2& operator=(Queue2&& other) noexcept = default;
    // методы очереди
    bool empty() noexcept;
    size_type size() const noexcept;
    reference front();
    reference back();
    void push(const_reference);
    void pop();
    template <typename Y> friend bool operator==(const Queue2<Y> &a, const Queue2<Y> &b);
    template <typename Y> friend bool operator!=(const Queue2<Y> &a, const Queue2<Y> &b);
};

// реализация

template <typename T>
Queue2<T>::Queue2() : Array<T>() {}

template <typename T>
Queue2<T>::Queue2(const Queue2 &other) : Array<T>(other){}
template <typename T>
Queue2<T>::Queue2(Queue2 &&other) noexcept : Array<T>(std::move(other)){}
template <typename T>
bool Queue2<T>::empty() noexcept
{
    return Array<T>::empty();
}

template<class T>
typename Queue2<T>::size_type Queue2<T>::size() const noexcept
{
    return Array<T>::size();
}

template<class T>
typename Queue2<T>::reference Queue2<T>::front()
{
    return Array<T>::front();
}

template<class T>
typename Queue2<T>::reference Queue2<T>::back()
{
    return Array<T>::back();
}

template<class T>
void Queue2<T>::push(Queue2::const_reference b)
{
    return Array<T>::push_back(b);
}

template<class T>
void Queue2<T>::pop()
{
    return Array<T>::pop_front();
}
template<class T>
bool operator==(const Queue2<T> &a, const Queue2<T> &b)
{
    if (a.size() != b.size()) return false;
    return std::equal(a.begin(),a.end(),b.begin());
}

template<class T>
bool operator!=(const Queue2<T> &a, const Queue2<T> &b)
{
    return !(a == b);
}


#endif // QUEUE2_H
