#include "queue.h"
#include <cmath>
#include <random>
#include <unordered_set>
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

// получить итератор на первый элемент очереди
Queue::iterator Queue::begin() noexcept
{
    return Lodger_array.begin();
}

// получить итератор указывающий после последнего элемента очереди
Queue::iterator Queue::end() noexcept
{
    return Lodger_array.end();
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

// удаление одного элемента по итератору
void Queue::erase(const iterator &it)
{
    size_t size_before = Lodger_array.size();
    Lodger_array.erase(it);
    if (size_before-1 == Lodger_array.size())
    {
        --count_of_people;
    }
}

// удаление нескольких элементов по диапазону
void Queue::erase(const iterator &first, const iterator &last)
{
    size_t size_before = Lodger_array.size();
    Lodger_array.erase(first,last);
    count_of_people -= ( size_before - Lodger_array.size() );
}

// замена элемента (pos - какой заменяем, lodger - на какой заменяем)
void Queue::replace(const iterator &pos, const Lodger &lodger)
{
    *pos = lodger;
}

// count - количество сгенерированных элементов
void Queue::generate_random_data(size_t count)
{
    time_t seed = time(nullptr);
    std::mt19937 mt(seed);
    // диапазоны для рандома
    std::uniform_int_distribution<uint32_t> id(1,99999999); // ID до 8 символов
    std::uniform_int_distribution<uint8_t> family_count(1,20); // до 20 детей в семье, к примеру
    std::uniform_int_distribution<uint16_t> year(2000,2020);
    std::uniform_int_distribution<uint8_t> month(1,12);
    std::uniform_real_distribution<float> area_occupied(0,100); // пусть до 100 кв метров
    std::uniform_int_distribution<uint8_t> rooms_count(1,12);
    std::uniform_real_distribution<float> area_required(0,200);
    // сгенерируем неповторяющиеся уникальные ID
    std::unordered_set<int> set;
    while (set.size() < count) set.insert(id(mt));
    for (size_t i = 0; i < count; ++i)
    {
        auto set_iterator = set.begin();
        float old_area = area_occupied(mt);
        float new_area = area_required(mt);
        while (new_area <= old_area) new_area = area_required(mt);
        std::advance(set_iterator,i);
        Lodger lodg(*set_iterator,family_count(mt),year(mt),month(mt),old_area,rooms_count(mt),new_area);
        Lodger_array.push_back(lodg);
    }
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

// булева функция, условие-сравнение при сортировке
bool Queue::uslovie_for_sort_by_date(const Lodger &a, const Lodger &b)
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

// сортировка по дате
void Queue::sort_by_date_std()
{
    std::sort(Lodger_array.begin(),Lodger_array.end(), [this](const Lodger &a, const Lodger &b)
    {
        return uslovie_for_sort_by_date(a,b);
    });
}

void Queue::sort_by_date()
{
    size_t N = Lodger_array.size();
    for (size_t i = 0; i < N-1; ++i)
    {
        size_t k = i;
        auto x = Lodger_array[i];
        for (size_t j = i+1; j < N; ++j)
        {
            if (uslovie_for_sort_by_date(Lodger_array[j],x))
            {
                k = j;
                x = Lodger_array[j];
            }
        }
        Lodger_array[k] = Lodger_array[i];
        Lodger_array[i] = x;
    }
}

// сортировка по требуемой площади
void Queue::sort_by_area_std()
{
    std::sort(Lodger_array.begin(),Lodger_array.end(), [this](Lodger &a, Lodger &b)
    {
        const auto area_a = get_Area_required(a);
        const auto area_b = get_Area_required(b);
        return area_a < area_b;
    });
}

void Queue::sort_by_area()
{
    size_t N = Lodger_array.size();
    for (size_t i = 0; i < N-1; ++i)
    {
        size_t k = i;
        auto x = Lodger_array[i];
        for (size_t j = i+1; j < N; ++j)
        {
            const float area_j = get_Area_required(Lodger_array[j]);
            const float area_x = get_Area_required(x);
            if (area_j < area_x)
            {
                k = j;
                x = Lodger_array[j];
            }
        }
        Lodger_array[k] = Lodger_array[i];
        Lodger_array[i] = x;
    }
}

// находим итератор элемента с датой, до или после которой мы будем искать элементы
Queue::iterator Queue::find_date_iterator(uint16_t Year, uint8_t Month)
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
    auto it = find_date_iterator(Year,Month);
    if (it == Lodger_array.begin()) throw element_not_found();
    return std::vector<Lodger>{Lodger_array.begin(),it};
}

// поиск по дате (после заданной даты)
std::vector<Queue::Lodger> Queue::find_after_date(uint16_t Year, uint8_t Month)
{
    auto it = find_date_iterator(Year,Month);
    auto it_end = Lodger_array.end();
    // если у элемента дата равна заданной, то я исключаю этот элемент из результирующего контейнера
    // так как нас интересуют элементы после заданной даты, а не с заданной датой
    if (it != it_end)
    {
        if (Year == get_Year(*it) && Month == get_Month(*it)) ++it;
    }

    if (it == it_end) throw element_not_found();
    return std::vector<Lodger>{it, Lodger_array.end()};
}

// находим итератор элемента с требуемой площадью, после которой мы будем искать элементы (включая элемент на который указывает итератор)
Queue::iterator Queue::find_area_iterator(float Area_required)
{
    auto area_predicate = [this, Area_required](const Lodger &lodger) -> bool
    {
        return get_Area_required(lodger) >= Area_required;
    };

    sort_by_area();
    auto it = std::find_if(Lodger_array.begin(),Lodger_array.end(), area_predicate);
    return it;
}

// поиск по требуемой площади (не меньше заданной)
std::vector<Queue::Lodger> Queue::find_by_area(float Area_required)
{
    auto it = find_area_iterator(Area_required);
    if (it == Lodger_array.end()) throw element_not_found();
    return std::vector<Lodger>{it, Lodger_array.end()};
}

// примитивный перевод значений lodger в строку
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
