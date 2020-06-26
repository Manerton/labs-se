#include "unemployed.h"

Unemployed::Unemployed(const std::string &_surname, const char _date[], const UnemployedElem &a) : List (1,a), surname(_surname)
{
    std::copy(_date, _date + strlen(_date), date);
}

Unemployed::Unemployed(const std::string &_surname, const char _date[], const std::initializer_list<List::value_type> &t) : List (t), surname(_surname)
{
    std::copy(_date, _date + strlen(_date), date);
}

Unemployed::Unemployed(const Unemployed &other) : List(other), surname(other.surname)
{
    std::copy(other.date, other.date + strlen(other.date), date);
}

Unemployed::Unemployed(Unemployed &&other) : List(std::move(other)), surname(other.surname)
{
    std::move(other.date, other.date + strlen(other.date), date);
    other.surname = "";
}

void Unemployed::add(const UnemployedElem &a)
{
    push_back(a);
}

List<Unemployed::value_type>::iterator<Unemployed::value_type> Unemployed::find_by_ID(const char *ID)
{
    for (auto it = begin(); !it.isNullptr(); ++it)
    {
        if (strcmp((*it).getID(),ID) == 0)
        {
            return it;
        }
    }
    return iterator<Unemployed::value_type>(nullptr);
}

const_iterator<UnemployedElem> Unemployed::find_by_ID(const char *ID) const
{
    for (auto it = begin(); !it.isNullptr(); ++it)
    {
        if (strcmp((*it).getID(),ID) == 0)
        {
            return it;
        }
    }
    return const_iterator<Unemployed::value_type>(nullptr);
}

List<Unemployed::value_type>::value_type Unemployed::find_elem_by_ID(const char *ID) const
{
    auto it = find_by_ID(ID);
    return *it;
}

List<Unemployed::value_type>::iterator<Unemployed::value_type> Unemployed::find_greater_than_seniority(const UnemployedElem::Seniority &b)
{
    for (auto it = begin(); it != end(); ++it)
    {
        UnemployedElem::Seniority temp = (*it).getFull_seniority();
        if (temp.year == b.year)
        {
            if (temp.month > b.month) return it;
        }
        if (temp.year > b.year) return it;
    }
    return iterator<Unemployed::value_type>(nullptr);
}

List<Unemployed::value_type>::iterator<Unemployed::value_type> Unemployed::find_less_than_seniority(const UnemployedElem::Seniority &b)
{
    for (auto it = begin(); it != end(); ++it)
    {
        UnemployedElem::Seniority temp = (*it).getFull_seniority();
        if (temp.year == b.year)
        {
            if (temp.month < b.month) return it;
        }
        if (temp.year < b.year) return it;
    }
    return iterator<Unemployed::value_type>(nullptr);
}

bool Unemployed::delete_by_ID(const char *ID)
{
    auto it = find_by_ID(ID);
    if (it.ptr != nullptr)
    {
        erase(it);
        return true;
    }
    return false;

}

bool Unemployed::change_by_ID(const char *ID, const List::value_type &b)
{
    auto it = find_by_ID(ID);
    if (it.ptr != nullptr)
    {
        *it = b;
        return true;
    }
    return false;
}
