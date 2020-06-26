#ifndef UNEMPLOYEDELEM_H
#define UNEMPLOYEDELEM_H
#include <object.h>
#include <string>
#include <cstring>
#include <cstdint>
#include <initializer_list>

// безработный
class UnemployedElem : public Object
{
public:
    struct Seniority                            // трудовой стаж
    {
        uint16_t year = 0;                      // количество лет
        uint8_t month = 0;                      // количество месяцев
    };
private:
    char ID[8] = "";                            // идентификационный номер
    uint8_t age = 0;                            // возраст
    bool gender = 0;                            // пол: 0 - мужской, 1 - женский
    Seniority last_seniority {0,0};             // стаж в последней должности
    Seniority full_seniority {0,0};             // общий стаж
    float salary;                               // желаемая заработная плата
public:
    UnemployedElem () = default;
    explicit UnemployedElem (const char _ID[8], const uint8_t _age, const bool _gender, const Seniority& _last_seniority, const Seniority& _full_seniority, const float _salary) noexcept
        :   Object(), age(_age), gender(_gender), last_seniority(_last_seniority), full_seniority(_full_seniority), salary(_salary)
    {
        std::copy(_ID, _ID + strlen(_ID), ID);
    }
    UnemployedElem(const UnemployedElem &other) noexcept;
    UnemployedElem(UnemployedElem &&other) noexcept;
    UnemployedElem& operator=(const UnemployedElem &other) noexcept = default;
    UnemployedElem& operator=(UnemployedElem &&other) noexcept = default;

    std::string toString() const noexcept;
    void clear() noexcept;
    const char* getID() const;
    Seniority getFull_seniority() const;

    // сравнения на равенство
    friend bool operator==(const UnemployedElem &a, const UnemployedElem &b);
    friend bool operator!=(const UnemployedElem &a, const UnemployedElem &b);
    ~UnemployedElem() { clear(); }
};

#endif // UNEMPLOYEDELEM_H
