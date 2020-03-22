#ifndef QUEUE_H
#define QUEUE_H
#include <string>
#include <cinttypes>
#include <algorithm>
#include <cstring>
#include <vector>
#include <tuple>

// -- очередь нуждающихся в улучшении жилищных условий -- //
class Queue
{
    using Lodger = std::tuple<
    char[8],    // ID
    uint32_t,   // Family_member_count
    uint16_t,   // Year
    uint8_t,    // Month
    float,      // Area_occupied
    uint8_t,    // Rooms_count
    float       // Area_required
    >; // -- квартирант/очередник -- //

    enum class LodgerIndex{
        ID,                     // идентификационный номер очередника
        Family_member_count,    // количество членов семьи
        Year,                   // год
        Month,                  // месяц постановки в очередь
        Area_occupied,          // занимаемая жилая площадь
        Rooms_count,            // необходимые жилищные условия (количество комнат)
        Area_required           // необходимые жилищные условия (требуемая площадь)
    };

    std::string name; // -- фамилия сотрудника, ведущего очередь -- //
    uint32_t count_of_people; // -- количество очередников -- //
    char date[8]; // -- дата создания -- //
    std::vector<Lodger> Lodger_array;
public:
    Queue(const std::string& _name = "noname", uint32_t _count_of_people = 0, const char _date[8] = "01.01.00") noexcept
        : name(_name), count_of_people(_count_of_people)
    {
        std::copy(_date,_date+strlen(_date),date);
    }

};

#endif // QUEUE_H
