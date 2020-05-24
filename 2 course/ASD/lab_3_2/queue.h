#ifndef QUEUE_H
#define QUEUE_H
#include <string>
#include <cinttypes>
#include <algorithm>
#include <cstring>
#include <vector>
#include <tuple>
#include <array>

// -- очередь нуждающихся в улучшении жилищных условий -- //
class Queue
{
   public:
    // кортеж Tuple
    using Lodger = std::tuple<
    uint32_t,    // ID
    uint8_t,   // Family_member_count
    uint16_t,   // Year
    uint8_t,    // Month
    float,      // Area_occupied
    uint8_t,    // Rooms_count
    float       // Area_required
    >; // -- квартирант/очередник -- //

    using iterator = std::vector<Lodger>::iterator;

    enum class LodgerIndex : uint8_t {
        ID,                     // идентификационный номер очередника
        Family_member_count,    // количество членов семьи
        Year,                   // год
        Month,                  // месяц постановки в очередь
        Area_occupied,          // занимаемая жилая площадь
        Rooms_count,            // необходимые жилищные условия (количество комнат)
        Area_required           // необходимые жилищные условия (требуемая площадь)
    };

private:
    std::string name; // -- фамилия сотрудника, ведущего очередь -- //
    uint32_t count_of_people = 0; // -- количество очередников -- //
    char date[9]; // -- дата создания в формате ДД.ММ.ГГ -- //
    std::vector<Lodger> Lodger_array;

    // -- главные проверки выбрасывают исключения, поэтому главные проверки имеют тип void -- //
    void check_date_format(const char* date) const; // -- формат даты должен быть ДД.ММ.ГГ и не больше 8 символов -- //
    bool check_for_digits_in_date(const char *date) const noexcept; // -- проверка на лишние символы (т.е не цифры) внутри даты -- //
    void check_length_for_ID(uint32_t ID) const; // -- проверка на длину ID, не должно быть больше 8 символов -- //

    // Сортировка для поисков
    void sort_by_date();
    iterator find_date(uint16_t Year, uint8_t Month); // находим итератор элемента с датой, до или после которой мы будем искать элементы
public:
    Queue(const std::string& _name = "noname", const char* _date = "01.01.00")
        : name{_name}
    {
        check_date_format(_date);
        strcpy(date,_date);
    }

    // -- геты -- //
    std::string get_Name() const noexcept;
    uint32_t get_Count_of_people() const noexcept;
    std::string get_Date() const noexcept;
    uint32_t get_ID(const Lodger& lodger) const noexcept;
    uint8_t get_Family_member_count(const Lodger& lodger) const noexcept;
    uint16_t get_Year(const Lodger& lodger) const noexcept;
    uint8_t get_Month(const Lodger& lodger) const noexcept;
    float get_Area_occupied(const Lodger& lodger) const noexcept;
    uint8_t get_Rooms_count(const Lodger& lodger) const noexcept;
    float get_Area_required(const Lodger& lodger) const noexcept;

    // -- основные операции -- //
    void add(const Lodger& lodger); // добавление

    iterator find_by_ID(uint32_t ID); // поиск по ID, возвращает итератор на элемент
    std::vector<Lodger> find_before_date(uint16_t Year, uint8_t Month); // поиск по дате (до заданной даты), возвращает контейнер с элементами
    std::vector<Lodger> find_after_date(uint16_t Year, uint8_t Month); // поиск по дате (после заданной даты)

    std::string lodger_toString(const Lodger &lodger) const noexcept;

    // -- исключения -- //

    class wrong_data_format {};     // неправильный формат даты
    class ID_out_of_bounds {};      // у ID больше 8 символов
    class element_not_found {};     // элемент в контейнере не найден

};

#endif // QUEUE_H
