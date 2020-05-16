#include "queue.h"
#include <cmath>

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

std::string Queue::getName() const
{
    return name;
}

uint32_t Queue::getCount_of_people() const
{
    return count_of_people;
}

const std::string Queue::getDate() const
{
    return std::string{date};
}

uint32_t Queue::getID_from_lodger(const Queue::Lodger &lodger) const
{
    const auto indexID = static_cast<int>(LodgerIndex::ID);
    return std::get<indexID>(lodger);
}

void Queue::add(const Queue::Lodger &lodger)
{
    // проверим ID на допустимость
    // а вообще надо все параметры проверить на допустимость

    uint32_t ID = getID_from_lodger(lodger);
    check_length_for_ID(ID);

    Lodger_array.push_back(lodger);
    ++count_of_people;
}
