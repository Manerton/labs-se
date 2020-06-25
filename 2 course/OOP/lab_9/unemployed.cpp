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

List::iterator Unemployed::find_by_ID(const char *ID)
{
    for (auto it = begin(); it != nullptr; it = it->next)
    {
        if (strcmp(it->N.getID(),ID) == 0)
        {
            return it;
        }
    }
    return nullptr;
}

List::iterator Unemployed::find_by_ID(const char *ID) const
{
    for (auto it = begin(); it != nullptr; it = it->next)
    {
        if (strcmp(it->N.getID(),ID) == 0)
        {
            return it;
        }
    }
    return nullptr;
}

List::value_type Unemployed::find_elem_by_ID(const char *ID) const
{
    auto it = find_by_ID(ID);
    return it->N;
}

List::iterator Unemployed::find_greater_than_seniority(const UnemployedElem::Seniority &b)
{
    for (auto it = begin(); it != nullptr; it = it->next)
    {
        UnemployedElem::Seniority temp = it->N.getFull_seniority();
        if (temp.year == b.year)
        {
            if (temp.month > b.month) return it;
        }
        if (temp.year > b.year) return it;
    }
    return nullptr;
}

List::iterator Unemployed::find_less_than_seniority(const UnemployedElem::Seniority &b)
{
    for (auto it = begin(); it != nullptr; it = it->next)
    {
        UnemployedElem::Seniority temp = it->N.getFull_seniority();
        if (temp.year == b.year)
        {
            if (temp.month < b.month) return it;
        }
        if (temp.year < b.year) return it;
    }
    return nullptr;
}

bool Unemployed::delete_by_ID(const char *ID)
{
    auto it = find_by_ID(ID);
    if (it != nullptr)
    {
        erase(it);
        return true;
    }
    return false;

}

bool Unemployed::change_by_ID(const char *ID, const List::value_type &b)
{
    auto it = find_by_ID(ID);
    if (it != nullptr)
    {
        it->N = b;
        return true;
    }
    return false;
}
