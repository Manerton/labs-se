template <typename T>
const_iterator<T> List<T>::begin() const noexcept
{
    return const_iterator<T>(head);
}

template <typename T>
const_iterator<T> List<T>::end() const noexcept
{
    return const_iterator<T>(tail);
}

template <typename T>
typename List<T>::reference List<T>::front()
{
    if(_size != 0) return head->N;
    throw std::length_error("Список пуст.");
}
template <typename T>
typename List<T>::reference List<T>::back()
{
    if(_size != 0) return tail->N;
    throw std::length_error("Список пуст.");
}
template <typename T>
List<T>::List(List::size_type n, List::const_reference v)
{
    if (n != 0)
    {
        for (size_type i = 0; i < n; ++i)
        {
            push_back(v);
        }
    }
}
template <typename T>
List<T>::List(const std::initializer_list<List::value_type> &t)
{
    if (t.size() != 0)
    {
        for (auto& v : t)
        {
            push_back(v);
        }
    }
}
template <typename T>
List<T>::List(const List<T> &other)
{
    if (!other.empty())
    {
        for(auto it = other.head; it != nullptr; it = it->next)
        {
            push_back(it->N);
        }
    }
}
template <typename T>
List<T>::List(List<T> &&other) noexcept : _size(other.size()), head(other.head), tail(other.tail)
{
    other._size = 0;
    other.head = nullptr;
    other.tail = nullptr;
}
template <typename T>
List<T> &List<T>::operator=(const List<T> &other) // -- по Саттеру -- //
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
template <typename T>
List<T> &List<T>::operator=(List<T> &&other) noexcept
{
    if (this != &other)
    {
        std::swap(_size,other._size);
        std::swap(head,other.head);
        std::swap(tail,other.tail);
    }
    return *this;
}
template <typename T>
List<T>::~List()
{
    clear();
}
template <typename T>
void List<T>::push_front(List::const_reference N)
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
template <typename T>
void List<T>::pop_front()
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
template <typename T>
void List<T>::push_back(List::const_reference N)
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
template <typename T>
void List<T>::pop_back()
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
template <typename T>
typename List<T>::template iterator<T> List<T>::insert(List::iterator<T> it, List::const_reference b)
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
    return iterator<T>(ptr);
}
template <typename T>
typename List<T>::template iterator<T> List<T>::erase(List::iterator<T> it)
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
    return iterator<T>(ptr);
}
template <typename T>
void List<T>::clear()
{
    while (!empty()) pop_front();
}
template <typename T>
void List<T>::swap(List<T> &t) noexcept
{
    if (head != t.head)
    {
        std::swap(_size,t._size);
        std::swap(head,t.head);
        std::swap(tail,t.tail);
    }
}
// операции сравнения для списка
template <typename T>
bool operator==(const List<T> &a, const List<T> &b)
{
    if (a.size() != b.size()) return false;
    for(auto it1 = a.head, it2 = b.head; it1 != nullptr; it1 = it1->next, it2 = it2->next)
    {
        if ( (it1->N) != (it2->N) ) return false;
    }
    return true;
}
template <typename T>
bool operator!=(const List<T> &a, const List<T> &b)
{
    return !(a == b);
}
// для узлов
template <typename T>
bool operator==(const typename List<T>::Node &a, const typename List<T>::Node &b)
{
    return (a.N == b.N);
}
template <typename T>
bool operator!=(const typename List<T>::Node &a, const typename List<T>::Node &b)
{
    return !(a == b);
}
template <typename T> template <typename Y>
// вложенный итератор
List<T>::iterator<Y>::iterator(List<T>::Node *t) noexcept
{
    ptr = t;
}
template <typename T> template <typename Y>
bool List<T>::iterator<Y>::operator==(const List<T>::iterator<Y> &it) const noexcept
{
    return (ptr == it.ptr);
}
template <typename T> template <typename Y>
bool List<T>::iterator<Y>::operator!=(const List<T>::iterator<Y> &it) const noexcept
{
    return !(*this == it);
}
template <typename T> template <typename Y>
bool List<T>::iterator<Y>::isNullptr() const noexcept
{
    return (ptr == nullptr);
}
template <typename T> template <typename Y>
typename List<T>::template iterator<Y> &List<T>::iterator<Y>::operator++()
{
    ptr = ptr->next;
    return *this;
}
template <typename T> template <typename Y>
typename List<T>::template iterator<Y> &List<T>::iterator<Y>::operator--()
{
    ptr = ptr->prev;
    return *this;
}
template <typename T> template <typename Y>
typename List<T>::reference List<T>::iterator<Y>::operator*()
{
    return ptr->N;
}
// внешний итератор (для чтения)
template <typename Y>
const_iterator<Y>::const_iterator(Node *t) noexcept
{
    ptr = t;
}
template <typename Y>
bool const_iterator<Y>::operator==(const const_iterator &it) const noexcept
{
    return (ptr == it.ptr);
}
template <typename Y>
bool const_iterator<Y>::operator!=(const const_iterator &it) const noexcept
{
    return !(*this == it);
}
template <typename Y>
const_iterator<Y> &const_iterator<Y>::operator++()
{
    ptr = ptr->next;
    return *this;
}
template <typename Y>
const_iterator<Y> &const_iterator<Y>::operator--()
{
    ptr = ptr->prev;
    return *this;
}
template <typename Y>
typename const_iterator<Y>::const_reference const_iterator<Y>::operator*()
{
    return ptr->N;
}
template <typename Y>
bool const_iterator<Y>::isNullptr() const noexcept
{
    return (ptr == nullptr);
}
