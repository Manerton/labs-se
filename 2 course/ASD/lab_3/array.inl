template <typename T>
void Array<T>::CheckZeroSize(Array::size_type n) const
{
    if (!n) throw std::length_error{"bad index exception"};
}
template <typename T>
void Array<T>::CheckIndex(size_type index) const
{
    if (index >= _size) throw std::out_of_range{"bad index exception"};
}
template <typename T>
Array<T>::Array(size_type n, const_reference t)
{
    CheckZeroSize(n);
    _size = n;
    data = new value_type[_size];
    std::fill(begin(),end(),t);
}
template <typename T>
Array<T>::Array(const std::initializer_list<value_type> &t) : Array(t.size())   // -- делегируем конструктуру выше -- //
{
    std::copy(t.begin(),t.end(),begin());
}
template <typename T>
Array<T>::Array(const Array &other) : _size(other._size), data(new value_type[_size])
{
    std::copy(other.begin(),other.end(),begin());
}
template <typename T>
Array<T>::Array(const iterator &_begin, const iterator &_end)
{
    _size = _end-_begin;
    data = new value_type[_size];
    std::copy(_begin,_end,begin());
}
template <typename T>
Array<T>::Array(Array &&other) noexcept : _size(other._size), data(other.data)
{
    other._size = 0;
    other.data = nullptr;
}
template <typename T>
Array<T> &Array<T>::operator=(const Array &other) // -- по Саттеру -- //
{
    if (this != &other)
    {
        Array temp(other);
        std::swap(_size,temp._size);
        std::swap(data,temp.data);
    }
    return *this;
}
template <typename T>
Array<T> &Array<T>::operator=(Array &&other) noexcept
{
    if (this != &other)
    {
        std::swap(_size,other._size);
        std::swap(data,other.data);
    }
    return *this;
}
template <typename T>
typename Array<T>::reference Array<T>::front()
{
    if(!empty()) return *(begin());
    throw std::length_error("Массив пуст.");
}
template <typename T>
typename Array<T>::const_reference Array<T>::front() const
{
    if(!empty()) return *(begin());
    throw std::length_error("Массив пуст.");
}
template <typename T>
typename Array<T>::reference Array<T>::back()
{
    if(!empty()) return *(--end());
    throw std::length_error("Массив пуст.");
}
template <typename T>
typename Array<T>::const_reference Array<T>::back() const
{
    if(!empty()) return *(--end());
    throw std::length_error("Массив пуст.");
}
template <typename T>
typename Array<T>::reference Array<T>::operator[](size_type index)
{
    CheckIndex(index);
    return data[index];
}
template <typename T>
typename Array<T>::const_reference Array<T>::operator[](size_type index) const
{
    CheckIndex(index);
    return data[index];
}
template <typename T>
void Array<T>::push_front(Array::const_reference b)
{
    Array t(_size+1, b);   // -- создаю новый массив, у которого размер больше на 1 элемент -- //
    std::move(begin(),end(),++t.begin()); // -- перемещаю элементы из старого массива -- //
    *this = std::move(t);   // -- вызываю конструктор перемещения, меняю t и this местами, старый удаляется при выходе из этого метода -- //
}
template <typename T>
void Array<T>::pop_front()
{
    Array t(_size-1);   // -- создаю новый массив, у которого размер больше на 1 элемент -- //
    auto it = ++begin();
    std::move(it,end(),t.begin()); // -- перемещаю элементы из старого массива -- //
    *this = std::move(t);   // -- вызываю конструктор перемещения, меняю t и this местами, старый удаляется при выходе из этого метода -- //
}
template <typename T>
void Array<T>::push_back(const_reference b)
{
    Array t(_size+1, b);   // -- создаю новый массив, у которого размер больше на 1 элемент -- //
    std::move(begin(),end(),t.begin()); // -- перемещаю элементы из старого массива -- //
    *this = std::move(t);   // -- вызываю конструктор перемещения, меняю t и this местами, старый удаляется при выходе из этого метода -- //
}
template <typename T>
void Array<T>::pop_back()
{
    Array t(_size-1);   // -- создаю новый массив, у которого размер меньше на 1 элемент -- //
    auto it = --end();
    std::move(begin(),it,t.begin()); // -- перемещаю элементы из старого массива -- //
    *this = std::move(t);   // -- вызываю конструктор перемещения, меняю t и this местами, старый удаляется при выходе из этого метода -- //
}
template <typename T>
typename Array<T>::iterator Array<T>::insert(Array<T>::iterator it, Array::const_reference b)
{
    if (it == begin()) { push_front(b); }
    else if (it == end()) {  push_back(b); }
    else
    {
        Array t(_size+1,b);

        size_type dist = size_type(it.ptr - begin().ptr);    // расстояние между первым элементом и указанным итератором
        std::move(begin(),it,t.begin());                // -- перемещаю элементы из старого массива до указанного итератора -- //
        auto new_it = iterator(t.begin().ptr + dist + 1);          // итератор на тот же элемент, перед которым мы добавляем новый элемент, в новом массиве t
        std::move(it,end(),new_it);                   // перемещаем элементы справа от итератора (после итератора)
        *this = std::move(t);
        it = new_it;
    }
    return it;
}
template <typename T>
typename Array<T>::iterator Array<T>::insert(Array<T>::size_type i, Array<T>::const_reference b)
{
    auto it = iterator(data+i);
    if (i == 0) { push_front(b); }
    else if (i == _size-1) {  push_back(b); }
    else
    {
        Array t(_size+1,b);
        std::move(begin(),it,t.begin());                // -- перемещаю элементы из старого массива до указанного итератора -- //
        auto new_it = iterator(t.begin().ptr + i + 1);
        std::move(it,end(),new_it);                   // перемещаем элементы справа от итератора (после итератора)
        *this = std::move(t);
        it = new_it;
    }
    return it;
}
template <typename T>
typename Array<T>::iterator Array<T>::erase(Array<T>::iterator it)
{
    if (it == begin()) { pop_front(); }
    else if (it == end()) {  pop_back(); }
    else
    {
        Array t(_size-1);

        size_type dist = size_type(it.ptr - begin().ptr);    // расстояние между первым элементом и указанным итератором
        std::move(begin(),it,t.begin());                // -- перемещаю элементы из старого массива до указанного итератора -- //
        auto new_it = iterator(t.begin().ptr + dist);          // итератор на тот же элемент, перед которым мы добавляем новый элемент, в новом массиве t
        std::move(++it,end(),new_it);                   // перемещаем элементы справа от итератора (после итератора)
        *this = std::move(t);
        it = new_it;
    }
    return it;
}
template <typename T>
typename Array<T>::iterator Array<T>::erase(Array::size_type i)
{
    auto it = iterator(data+i);
    if (i == 0) { pop_front(); }
    else if (i == _size-1) {  pop_back(); }
    else
    {
        Array t(_size-1);
        std::move(begin(),it,t.begin());                // -- перемещаю элементы из старого массива до указанного итератора -- //
        auto new_it = iterator(t.begin().ptr + i);
        std::move(++it,end(),new_it);                   // перемещаем элементы справа от итератора (после итератора)
        *this = std::move(t);
        it = new_it;
    }
    return it;
}
template <typename T>
bool operator==(const Array<T> &a, const Array<T> &b)
{
    if (a._size != b._size) return false;
    return std::equal(a.begin(),a.end(),b.begin());
}
template <typename T>
bool operator!=(const Array<T> &a, const Array<T> &b)
{
    return !(a == b);
}
template <typename T>
// вложенный итератор
Array<T>::iterator::iterator(Array<T>::value_type *t) noexcept
{
    ptr = t;
}
template <typename T>
bool Array<T>::iterator::operator==(const Array<T>::iterator &it) const noexcept
{
    return (ptr == it.ptr);
}
template <typename T>
bool Array<T>::iterator::operator!=(const Array<T>::iterator &it) const noexcept
{
    return !(*this == it);
}
template <typename T>
typename Array<T>::iterator &Array<T>::iterator::operator++()
{
    ++ptr;
    return *this;
}
template <typename T>
typename Array<T>::iterator &Array<T>::iterator::operator--()
{
    --ptr;
    return *this;
}
template <typename T>
typename Array<T>::iterator Array<T>::iterator::operator++(int)
{
    auto it = *this;
    ++ptr;
    return it;
}
template <typename T>
typename Array<T>::iterator Array<T>::iterator::operator--(int)
{
    auto it = *this;
    --ptr;
    return it;
}
template <typename T>
typename Array<T>::reference Array<T>::iterator::operator*()
{
    return *ptr;
}
template <typename T>
bool Array<T>::iterator::isNullptr() const noexcept
{
    return (ptr == nullptr);
}
