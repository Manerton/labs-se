#ifndef QUEUE_H
#define QUEUE_H
#include <string>
#include <cinttypes>
#include <algorithm>
#include <cstring>
#include <tuple>
#include "list.h"

// -- очередь нуждающихся в улучшении жилищных условий -- //
class Queue
{
public:
    // кортеж Tuple
    using Lodger = std::tuple<
    uint64_t,    // ID
    uint8_t,   // Family_member_count
    uint16_t,   // Year
    uint8_t,    // Month
    float,      // Area_occupied
    uint8_t,    // Rooms_count
    float       // Area_required
    >; // -- квартирант/очередник -- //


    // -- синонимы типов –- //
    using size_type = size_t;                   // -- тип для размера -- //
    using value_type = Lodger;                 // -- ОСНОВНОЙ тип элементов -- //
    using reference = value_type&;              // -- тип ссылка -- //
    using const_reference = const value_type&;  // -- тип константная ссылка -- //

    using Container = List<value_type>;
    using iterator = Container::iterator<value_type>;

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
    Container Lodger_list;

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
    static uint64_t get_ID(const_reference b) noexcept;
    static uint8_t get_Family_member_count(const_reference b)  noexcept;
    static uint16_t get_Year(const_reference b)  noexcept;
    static uint8_t get_Month(const_reference b)  noexcept;
    static float get_Area_occupied(const_reference b)  noexcept;
    static uint8_t get_Rooms_count(const_reference b)  noexcept;
    static float get_Area_required(const_reference b)  noexcept;
    // итераторы
    iterator begin() const noexcept;
    iterator end() const noexcept;
    iterator begin() noexcept;
    iterator end() noexcept;
    iterator advance(iterator it, size_type i);
    static size_type distance(const iterator&, const iterator&) noexcept;
    // -- основные операции -- //
    void add(const_reference b); // добавление
    void erase(const iterator& pos); // удаление одного элемента по итератору
    void replace(iterator& pos, const_reference b); // замена элемента (pos - какой заменяем, lodger - на какой заменяем)
    // сортировка слиянием
    void mergeSort(size_type l, size_type r);
    // слить два отсортированных списка в один
    void merge(const Queue &other);
    // бинарный и интерполяционный поиск
    iterator binarySearch_by_ID(uint64_t ID) noexcept;
    iterator interpolarSearch_by_ID(uint64_t ID) noexcept;
    // Удалить дубликаты
    void unique();
    // случайно переставить элементы
    void shuffle();
    // разделяем список на два в соотвествии с критерием (больше или меньше)
    Queue split_by_ID(uint64_t ID);
    std::string lodger_toString(const_reference) const noexcept; // примитивный перевод значений lodger в строку

    // -- исключения -- //

    class wrong_data_format {};     // неправильный формат даты
    class ID_out_of_bounds {};      // у ID больше 8 символов
    class element_not_found {};     // элемент в контейнере не найден
};

void quick_sort(Queue::iterator begin, Queue::iterator left, Queue::iterator right);

#endif // QUEUE_H
