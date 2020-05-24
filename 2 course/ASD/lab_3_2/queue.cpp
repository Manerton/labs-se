#include "queue.h"
#include <cmath>
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

uint32_t Queue::get_Count_of_people() const noexcept
{
    return count_of_people;
}

std::string Queue::get_Date() const noexcept
{
    return std::string{date};
}

uint32_t Queue::get_ID(const Lodger &lodger) const noexcept
{
    const auto index = static_cast<int>(LodgerIndex::ID);
    return std::get<index>(lodger);
}

uint8_t Queue::get_Family_member_count(const Lodger &lodger) const noexcept
{
    const auto index = static_cast<int>(LodgerIndex::Family_member_count);
    return std::get<index>(lodger);
}

uint16_t Queue::get_Year(const Lodger &lodger) const noexcept
{
    const auto index = static_cast<int>(LodgerIndex::Year);
    return std::get<index>(lodger);
}

uint8_t Queue::get_Month(const Lodger &lodger) const noexcept
{
    const auto index = static_cast<int>(LodgerIndex::Month);
    return std::get<index>(lodger);
}

float Queue::get_Area_occupied(const Lodger &lodger) const noexcept
{
    const auto index = static_cast<int>(LodgerIndex::Area_occupied);
    return std::get<index>(lodger);
}

uint8_t Queue::get_Rooms_count(const Lodger &lodger) const noexcept
{
    const auto index = static_cast<int>(LodgerIndex::Rooms_count);
    return std::get<index>(lodger);
}

float Queue::get_Area_required(const Lodger &lodger) const noexcept
{
    const auto index = static_cast<int>(LodgerIndex::Area_required);
    return std::get<index>(lodger);
}

// добавление
void Queue::add(const Lodger &lodger)
{
    // проверим ID на допустимость

    uint32_t ID = get_ID(lodger);
    check_length_for_ID(ID);

    Lodger_array.push_back(lodger);
    ++count_of_people;
}

// поиск по ID, возвращает итератор на элемент
Queue::iterator Queue::find_by_ID(uint32_t ID)
{
    // лямбда функция для сравнения ID переданного в функцию поиска и ID проверяемого элемента типа Lodger
    auto is_ID_equal = [this, ID](const Lodger &lodger) -> bool
    {
        return (ID == get_ID(lodger));
    };
    auto it = std::find_if(Lodger_array.begin(),Lodger_array.end(), is_ID_equal);
    if (it == Lodger_array.end()) throw element_not_found();
    return it;
}

void Queue::sort_by_date()
{
    std::sort(Lodger_array.begin(),Lodger_array.end(), [this](Lodger &a, Lodger &b)
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
    });
}
// находим итератор элемента с датой, до или после которой мы будем искать элементы
Queue::iterator Queue::find_date(uint16_t Year, uint8_t Month)
{
    auto date_predicate = [this, Year, Month](const Lodger &lodger) -> bool
    {
        if (Year == get_Year(lodger))
        {
            return Month <= get_Month(lodger);
        }
        else { return Year < get_Year(lodger); }
    };

    sort_by_date();
    auto it = std::find_if(Lodger_array.begin(),Lodger_array.end(), date_predicate);
    return it;
}
// поиск по дате (до заданной даты), возвращает контейнер с элементами
std::vector<Queue::Lodger> Queue::find_before_date(uint16_t Year, uint8_t Month)
{
    auto it = find_date(Year,Month);
    return std::vector<Lodger>{Lodger_array.begin(),it};
}
// поиск по дате (после заданной даты)
std::vector<Queue::Lodger> Queue::find_after_date(uint16_t Year, uint8_t Month)
{
    auto it = find_date(Year,Month);
    // если у элемента дата равна заданной, то я исключаю этот элемент из результирующего контейнера
    // так как нас интересуют элементы после заданной даты, а не с заданной датой
    if (Year == get_Year(*it) && Month == get_Month(*it)) ++it;
    return std::vector<Lodger>{it, Lodger_array.end()};
}

std::string Queue::lodger_toString(const Lodger &lodger) const noexcept
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
