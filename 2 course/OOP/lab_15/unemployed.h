#ifndef UNEMPLOYED_H
#define UNEMPLOYED_H
#include <list.h>
#include <string>
#include <initializer_list>

class Unemployed : public List
{
    std::string surname = "";   // фамилия оператора
    size_t count = 0;           // количество безработных
    char date[8] = "";              // Дата создания DDMMYYYY
    /// Количество очередников
public:
    Unemployed() = default;
    Unemployed(const std::string &_surname, const char _date[8], const value_type &a);
    Unemployed(const std::string &_surname, const char _date[8], const std::initializer_list<value_type> &t);
    Unemployed(const Unemployed& other);
    Unemployed(Unemployed&& other);
    Unemployed& operator=(const Unemployed& other) = default;
    Unemployed& operator=(Unemployed&& other) noexcept = default;
    virtual ~Unemployed() {};

    void add(const UnemployedElem &a); // добавление

    // поиск
    iterator find_by_ID             (const char* ID);
    iterator find_by_ID             (const char* ID) const;
    value_type find_elem_by_ID      (const char* ID) const;
    iterator find_greater_than_seniority   (const value_type::Seniority& b);
    iterator find_less_than_seniority      (const value_type::Seniority& b);
    // удаление
    bool delete_by_ID               (const char* ID);

    // замена
    bool change_by_ID               (const char* ID,  const value_type &b);
};

#endif // UNEMPLOYED_H
