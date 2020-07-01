#ifndef QUEUE_H
#define QUEUE_H
#include <string>
#include <cinttypes>
#include <algorithm>
#include <cstring>
#include <tuple>
#include "array.h"

// -- очередь нуждающихся в улучшении жилищных условий -- //
class Queue
{
public:

    // -- синонимы типов –- //
    using Container = Array;
    using size_type = size_t;                   // -- тип для размера -- //
    using value_type = Array::value_type;
    using lodger = Array::lodger;
    using key_t = Array::key_t;
    using reference = value_type&;              // -- тип ссылка -- //
    using const_reference = const value_type&;  // -- тип константная ссылка -- //

    enum class LodgerIndex : uint8_t {
        Family_member_count,    // количество членов семьи
        Year,                   // год
        Month,                  // месяц постановки в очередь
        Area_occupied,          // занимаемая жилая площадь
        Rooms_count,            // необходимые жилищные условия (количество комнат)
        Area_required           // необходимые жилищные условия (требуемая площадь)
    };

private:
    std::string name; // -- фамилия сотрудника, ведущего очередь -- //
    size_type count_of_people = 0; // -- количество очередников -- //
    char date[9]; // -- дата создания в формате ДД.ММ.ГГ -- //
    Container Lodger_array;

    // -- главные проверки выбрасывают исключения, поэтому главные проверки имеют тип void -- //
    void check_date_format(const char* date) const; // -- формат даты должен быть ДД.ММ.ГГ и не больше 8 символов -- //
    bool check_for_digits_in_date(const char *date) const noexcept; // -- проверка на лишние символы (т.е не цифры) внутри даты -- //
    void check_length_for_ID(uint32_t ID) const; // -- проверка на длину ID, не должно быть больше 8 символов -- //

    bool uslovie_for_sort_by_date(const_reference a, const_reference b); // булева функция, условие-сравнение при сортировке по дате
public:
    Queue(const std::string& _name = "noname", const char* _date = "01.01.00")
        : name{_name}, Lodger_array(2)
    {
        check_date_format(_date);
        strcpy(date,_date);
    }

    // -- геты -- //
    std::string get_Name() const noexcept;
    size_type get_Count_of_people() const noexcept;
    std::string get_Date() const noexcept;
    static uint8_t get_Family_member_count(const_reference b)  noexcept;
    static uint16_t get_Year(const_reference b)  noexcept;
    static uint8_t get_Month(const_reference b)  noexcept;
    static float get_Area_occupied(const_reference b)  noexcept;
    static uint8_t get_Rooms_count(const_reference b)  noexcept;
    static float get_Area_required(const_reference b)  noexcept;
    // -- основные операции -- //
    void add(const key_t& key, const lodger& l); // добавление
    void add(const_reference b); // добавление
    void erase(const key_t& key); // удаление одного элемента по ключу
    void replace(const key_t& key, const_reference b); // замена элемента (key - какой заменяем, lodger - на какой заменяем)
    value_type find(const key_t& key);
    // -- исключения -- //

    class wrong_data_format {};     // неправильный формат даты
    class ID_out_of_bounds {};      // у ID больше 8 символов
};

#endif // QUEUE_H
