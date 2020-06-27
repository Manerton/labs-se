#include "queue.h"
#include <cmath>
#include <random>
#include <unordered_set>
#include <iostream>
#include <iterator>
#include <ctime>
#include <sstream>

// -- проверка на цифры внутри даты -- //
bool Queue::check_for_digits_in_date(const char *date) const noexcept
{
    size_t len = strlen(date);
    for (size_t i = 0; i < len; ++i)
    {
        if (i != 2 && i != 5) // пропускаем точки
        {
            if (!isdigit(date[i])) return false;
        }
    }
    return true;

}

// -- формат даты должен быть ДД.ММ.ГГ и не больше 8 символов -- //
void Queue::check_date_format(const char *date) const
{
    // макс. значения
    const uint8_t date_len = 8; // 8 символов в дате
    const uint8_t max_days = 31;
    const uint8_t max_month = 12;

    // проверка на длину
    const bool check_for_len = (strlen(date) == date_len);
    // проверка на точки
    const bool check_for_dots = ((date[2] == '.') && (date[2] == date[5]));
    // проверка на цифры
    const bool check_for_digits = check_for_digits_in_date(date);

    if (!check_for_len || !check_for_dots || !check_for_digits) throw wrong_data_format();

    int day = stoi(std::string(date).substr(0,2));
    int month = stoi(std::string(date).substr(3,2));

    if ((day > max_days) || (month > max_month)) throw wrong_data_format();
}

// -- проверка на длину ID, не должно быть больше 8 символов -- //
void Queue::check_length_for_ID(uint32_t ID) const
{
    const uint8_t max_id_len = 8;
    uint8_t id_len = 1;
    if (ID != 0)
    {
        id_len = ceil(log10((ID)+0.5)); // -- количество цифр в числе -- //
    }
    if (id_len > max_id_len) throw ID_out_of_bounds();
}

std::string Queue::get_Name() const noexcept
{
    return name;
}

Queue::size_type Queue::get_Count_of_people() const noexcept
{
    return count_of_people;
}

std::string Queue::get_Date() const noexcept
{
    return std::string{date};
}

uint64_t Queue::get_ID(Queue::const_reference lodger) noexcept
{
    const auto index = static_cast<int>(LodgerIndex::ID);
    return std::get<index>(lodger);
}

uint8_t Queue::get_Family_member_count(Queue::const_reference lodger) noexcept
{
    const auto index = static_cast<int>(LodgerIndex::Family_member_count);
    return std::get<index>(lodger);
}

uint16_t Queue::get_Year(Queue::const_reference lodger)  noexcept
{
    const auto index = static_cast<int>(LodgerIndex::Year);
    return std::get<index>(lodger);
}

uint8_t Queue::get_Month(Queue::const_reference lodger) noexcept
{
    const auto index = static_cast<int>(LodgerIndex::Month);
    return std::get<index>(lodger);
}

float Queue::get_Area_occupied(Queue::const_reference lodger) noexcept
{
    const auto index = static_cast<int>(LodgerIndex::Area_occupied);
    return std::get<index>(lodger);
}

uint8_t Queue::get_Rooms_count(Queue::const_reference lodger) noexcept
{
    const auto index = static_cast<int>(LodgerIndex::Rooms_count);
    return std::get<index>(lodger);
}

float Queue::get_Area_required(Queue::const_reference lodger) noexcept
{
    const auto index = static_cast<int>(LodgerIndex::Area_required);
    return std::get<index>(lodger);
}

auto Queue::begin() const noexcept
{
    return Lodger_list.begin();
}

auto Queue::end() const noexcept
{
    return Lodger_list.end();
}

// получить итератор на первый элемент очереди
Queue::iterator Queue::begin() noexcept
{
    return Lodger_list.begin();
}

// получить итератор указывающий после последнего элемента очереди
Queue::iterator Queue::end() noexcept
{
    return Lodger_list.end();
}

Queue::iterator Queue::advance(iterator it, size_type i)
{
    if (i == 0) return it;
    size_type counter = 0;

    while (it != end() && counter != i) // выход из цикла либо дошли до end, либо дошли необходимое количество раз
    {
        if (i > 0) { counter++; ++it;}
        else {counter--; --it;}
    }
    if (it == end()) throw std::out_of_range("Превышение количества элементов!");
    return it;
}

Queue::size_type Queue::distance(const Queue::iterator &a, const Queue::iterator &b) noexcept
{
    size_type result = 0;
    iterator first(a);
    if (first == b) return 0; // если равны

    while (first != b)  // пока не дойдем до второго итератора
    {
        ++result; // увеличиваем количество пройденного
        ++first; // двигаем итератор
    }
    return result;
}

// добавление
void Queue::add(Queue::const_reference lodger)
{
    // проверим ID на допустимость

    uint32_t ID = get_ID(lodger);
    check_length_for_ID(ID);

    Lodger_list.push_back(lodger);
    ++count_of_people;
}

// удаление одного элемента по итератору
void Queue::erase(const iterator &it)
{
    size_t size_before = Lodger_list.size();
    Lodger_list.erase(it);
    if (size_before-1 == Lodger_list.size())
    {
        --count_of_people;
    }
}

void Queue::mergeSort(size_type l, size_type r)
{
    // лямбды функции для сокращения кода в самой сортировке
    // it возвращает итератор на i-том месте
    auto it = [this](const size_type i) -> iterator
    {
        return advance(begin(),i);
    };
    // ID возвращает ID у итератора на i-том месте
    auto ID = [it](const size_type i) -> uint64_t
    {
        return get_ID(*it(i));
    };

    if (l != r) // если границы не сомкнулись
    {
        size_type mid = (l + r) / 2;
        mergeSort(l, mid);
        mergeSort(mid+1, r);
        size_type i = l;  // начало первого пути
        size_type j = mid + 1; // начало второго пути
        Container tmp; // дополнительный список
        for (size_type step = 0; step < (r - l + 1); ++step) // для всех элементов дополнительного контейнера
        {
            // записываем в формируемую последовательность меньший из элементов двух путей
            // или остаток первого пути если j > r
            if ((j > r) || ((i <= mid) && ( ID(i) < ID(j) )))
            {
                tmp.push_back(*it(i));
                ++i;
            }
            else
            {
                tmp.push_back(*it(j));
                ++j;
            }
        }
        for (size_type step = 0; step < (r - l + 1); ++step)
        {
            *(advance(begin(),l+step)) = *(advance(tmp.begin(),step));
        }
        tmp.clear(); // удаляем дополнительный список
    }
}

void Queue::merge(const Queue &other)
{
    if (this != &other) // если не сливаем с самим собой
    {
        // получаем итераторы начала исходного списка и другого
        auto it_this = begin();
        auto it_other = other.begin();
        while (it_other != other.end()) // пока не дошли до конца другого списка
        {
            // в исходном списке элемент под итератором оказался >=, то вставляем элемент из другого списка перед ним
            if (get_ID(*it_this) >= get_ID(*it_other))
            {
                Lodger_list.insert(it_this,*it_other);
                ++count_of_people;
                it_this--;  // в исходном списке перемещаемся на только что добавленный
                it_other++; // в другом списке идем к следующему
            }
            else // иначе оказалось что элемент в исходном списке < элемента из другого
            {
                // идем дальше по исходному списку на один элемент, если это не последний элемент
                if (it_this != --end())
                {
                    ++it_this;
                }
                else
                {   // это последний элемент, вставляем элемент из другого списка в конец исходного
                    // и переходим к след. вставляемому элементу
                    add(*it_other);
                    it_other++;
                    it_this++;
                }
            }
        }
    }
}

// независимая функция быстрой сортировки
void quick_sort(Queue::iterator begin, Queue::iterator left, Queue::iterator right)
{
    Queue::value_type pivot = *left; // разрешающий элемент
    uint64_t ID = Queue::get_ID(pivot);
    // левая и правая границы
    Queue::iterator l_hold = left, r_hold = right;
    while (left != right) // пока границы не сомкнутся
    {
        while ((Queue::get_ID(*right) >= ID) && (left != right))
        {
            --right; // сдвигаем правую границу пока элемент [right] больше [pivot]
        }
        if (left != right) // если границы не сомкнулись
        {
            *left = *right;
            left++; // сдвигаем левую границу вправо
        }
        while (Queue::get_ID(*left) <= ID && (left != right))
            left++; // сдвигаем левую границу пока элемент [left] меньше [pivot]
        if (left != right) // если границы не сомкнулись
        {
            *right = *left;
            right--; // сдвигаем правую границу влево
        }
    }
    // сохраняем на месте, где сомкнулись left и right, разрешающий элемент
    *left = pivot;
    Queue::iterator pivotPtr = left;
    left = l_hold;
    right   = r_hold;
    if (Queue::get_ID(*left) < Queue::get_ID(*pivotPtr)) // Рекурсивно вызываем сортировку для левой и правой части массива
        quick_sort(begin, left, --pivotPtr);
    if (Queue::get_ID(*right) > Queue::get_ID(*pivotPtr))
        quick_sort(begin, ++pivotPtr, right);
}

// замена элемента (pos - какой заменяем, lodger - на какой заменяем)
void Queue::replace(const iterator &pos, const Lodger &lodger)
{
    *pos = lodger;
}

// булева функция, условие-сравнение при сортировке
bool Queue::uslovie_for_sort_by_date(const_reference a, const_reference b)
{
    const auto year_a = get_Year(a);
    const auto year_b = get_Year(b);
    if (year_a == year_b)
    {
        const auto month_a = get_Month(a);
        const auto month_b = get_Month(b);
        return month_a < month_b;
    }
    else { return year_a < year_b; }
}

Queue::iterator Queue::binarySearch_by_ID(uint64_t ID) noexcept
{
    auto left = begin(), right = --end();
    while (distance(begin(),left) <= distance(begin(),right))
    {
        size_type mid_index = (distance(begin(),left) +  distance(begin(),right)) / 2; // серединый индекс
        auto mid = advance(begin(),mid_index);  // получили итератор с помощью этого индекса
        if (get_ID(*mid) == ID) return (mid);
        if (ID < get_ID(*mid)) // если искомое меньше найденной середины
        {
            right = --mid;    // смещаем правую границу (ищем в левой части)
        }
        else
        {
            left = ++mid;    // наоборот (в правой ищем)
        }
    }
    return Lodger_list.end();
}

Queue::iterator Queue::interpolarSearch_by_ID(uint64_t ID) noexcept
{
    auto left = begin(), right = --end();
    while (get_ID(*left) <= ID && get_ID(*right) >= ID)
    {
        size_type dist_left = (distance(begin(),left));
        size_type dist_right = (distance(begin(),right));
        size_type mid_index = dist_left+(((ID-get_ID(*left))*(dist_right-dist_left))/((get_ID(*right))-get_ID(*left)));
        auto mid = advance(begin(),mid_index);  // получили итератор с помощью этого индекса
        if (get_ID(*mid) == ID) return (mid);
        if (ID < get_ID(*mid)) // если искомое меньше найденной середины
        {
            right = --mid;    // смещаем правую границу (ищем в левой части)
        }
        else
        {
            left = ++mid;    // наоборот (в правой ищем)
        }
    }
    return Lodger_list.end();
}

void Queue::unique()
{
    Lodger_list.unique();
    count_of_people = Lodger_list.size();
}

void Queue::shuffle()
{
    time_t seed = time(nullptr);
    std::mt19937 mt(seed);
    static std::uniform_int_distribution<int> rnd(0, Lodger_list.size()-1); // для генерации случайного + для итератора
    for (auto i = begin(); i != end(); ++i)
    {
        auto it = advance(begin(),rnd(mt));
        std::swap(*i,*it);
    }
}

Queue Queue::split_by_ID(uint64_t ID)
{
    Queue tmp("Результат поиска по ID", get_Date().c_str());
    // создаем новый список в котором будут элементы > ID
    // ну а в старом останутся элементы <= ID
    auto i = begin();
    while (i != end())
    {
        if (get_ID(*i) > ID)
        {
            auto toDelete = i;
            ++i;
            tmp.add(*toDelete); erase(toDelete);
        }
        else
        {
            ++i;
        }

    }
    return tmp;
}

// примитивный перевод значений lodger в строку
std::string Queue::lodger_toString(Queue::const_reference lodger) const noexcept
{
    std::stringstream ss;

    ss << get_ID(lodger) << " "
       << int(get_Family_member_count(lodger)) << " "
       << get_Year(lodger) << " "
       << int(get_Month(lodger)) << " "
       << get_Area_occupied(lodger) << " "
       << int(get_Rooms_count(lodger)) << " "
       << get_Area_required(lodger);

    std::string s(ss.str());
    return s;
}
