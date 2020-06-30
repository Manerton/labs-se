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

// добавление
void Queue::add(const key_t &key, const_reference lodger)
{
    // проверим ID на допустимость (8 цифр)
    check_length_for_ID(key);

    Lodger_tree.insert(key,lodger);
    ++count_of_people;
}

// удаление одного элемента по итератору
void Queue::erase(const key_t &key)
{
    Lodger_tree.erase(key);
    count_of_people = Lodger_tree.count();
}

// замена элемента (pos - какой заменяем, lodger - на какой заменяем)
void Queue::replace(const key_t &key, const_reference lodger)
{
    Lodger_tree.replace(key,lodger);
}

Queue::value_type Queue::find(const Queue::key_t &key)
{
    return Lodger_tree.get_value(Lodger_tree.find(key));
}
