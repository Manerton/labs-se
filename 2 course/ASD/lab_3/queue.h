#ifndef QUEUE_H
#define QUEUE_H
#include <string>
#include <cinttypes>
#include <algorithm>
#include <cstring>
#include "array.h"
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

    using iterator = Array<Lodger>::iterator; // сокращение
    // -- синонимы типов –- //
    using size_type = size_t;                   // -- тип для размера -- //
    using value_type = Lodger;                 // -- ОСНОВНОЙ тип элементов -- //
    using reference = value_type&;              // -- тип ссылка -- //
    using const_reference = const value_type&;  // -- тип константная ссылка -- //

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
    size_type count_of_people = 0; // -- количество очередников -- //
    char date[9]; // -- дата создания в формате ДД.ММ.ГГ -- //
    Array<value_type> Lodger_array;

    // -- главные проверки выбрасывают исключения, поэтому главные проверки имеют тип void -- //
    void check_date_format(const char* date) const; // -- формат даты должен быть ДД.ММ.ГГ и не больше 8 символов -- //
    bool check_for_digits_in_date(const char *date) const noexcept; // -- проверка на лишние символы (т.е не цифры) внутри даты -- //
    void check_length_for_ID(uint32_t ID) const; // -- проверка на длину ID, не должно быть больше 8 символов -- //

    bool uslovie_for_sort_by_date(const_reference a, const_reference b); // булева функция, условие-сравнение при сортировке по дате
public:
    Queue(const std::string& _name = "noname", const char* _date = "01.01.00")
        : name{_name}
    {
        check_date_format(_date);
        strcpy(date,_date);
    }

    // -- геты -- //
    std::string get_Name() const noexcept;
    size_type get_Count_of_people() const noexcept;
    std::string get_Date() const noexcept;
    uint32_t get_ID(const_reference b) const noexcept;
    uint8_t get_Family_member_count(const_reference b) const noexcept;
    uint16_t get_Year(const_reference b) const noexcept;
    uint8_t get_Month(const_reference b) const noexcept;
    float get_Area_occupied(const_reference b) const noexcept;
    uint8_t get_Rooms_count(const_reference b) const noexcept;
    float get_Area_required(const_reference b) const noexcept;
    iterator begin() noexcept;
    iterator end() noexcept;

    // -- основные операции -- //
    void add(const_reference b); // добавление
    void erase(const iterator& pos); // удаление одного элемента по итератору
    void replace(iterator& pos, const_reference b); // замена элемента (pos - какой заменяем, lodger - на какой заменяем)

    // заполнение очереди (контейнера случайными значениями)
    void generate_random_data(size_t count); // count - количество сгенерированных элементов

    // сортировки
    void sort_by_id();
    void sort_by_id_std();
    void sort_by_date();
    void sort_by_date_std();
    void sort_by_area();
    void sort_by_area_std();
    // эти поиски возвращают итератор, например для удаления одного или нескольких элементов (диапазона)
    iterator find_by_ID_std(uint32_t ID); // поиск по ID, возвращает итератор на элемент
    iterator find_date_iterator(uint16_t Year, uint8_t Month); // находим итератор элемента с датой, до или после которой мы будем искать элементы
    iterator find_area_iterator(float Area_required); // находим итератор элемента с требуемой площадью, после которой мы будем искать элементы (включая элемент на который указывает итератор)
    // линейные поиски с барьером
    iterator linearSearch_by_ID_unsorted(uint32_t ID) noexcept;
    iterator linearSearch_by_ID_sorted(uint32_t ID) noexcept;
    // бинарный и интерполяционный поиски
    iterator binarySearch_by_ID(uint32_t ID) noexcept;
    iterator interpolarSearch_by_ID(uint32_t ID) noexcept;
    // эти поиски возвращают контейнеры с элементами
    Array<value_type> find_before_date(uint16_t Year, uint8_t Month); // поиск по дате (до заданной даты), возвращает контейнер с элементами
    Array<value_type> find_after_date(uint16_t Year, uint8_t Month); // поиск по дате (после заданной даты)
    Array<value_type> find_by_area(float Area_required); // поиск по требуемой площади (не меньше заданной)

    std::string lodger_toString(const_reference) const noexcept; // примитивный перевод значений lodger в строку

    // -- исключения -- //

    class wrong_data_format {};     // неправильный формат даты
    class ID_out_of_bounds {};      // у ID больше 8 символов
    class element_not_found {};     // элемент в контейнере не найден
};

#endif // QUEUE_H
