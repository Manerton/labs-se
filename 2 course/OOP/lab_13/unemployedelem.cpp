#include "unemployedelem.h"
#include <sstream>

using namespace std;

string UnemployedElem::toString() const noexcept
{
    stringstream ss;
    ss << ID << " " << int(age) << " " << gender << " " << last_seniority.year << ";" << int(last_seniority.month) << " " << full_seniority.year << ";" << int(full_seniority.month) << " " << salary;
    return ss.str();
}

void UnemployedElem::clear() noexcept
{
    std::strcpy (ID,   "00000000");
    age = gender = last_seniority.year  = last_seniority.month = full_seniority.year = full_seniority.month = salary = 0;
}

const char *UnemployedElem::getID() const
{
    return ID;
}

UnemployedElem::Seniority UnemployedElem::getFull_seniority() const
{
    return full_seniority;
}

// Сравнения
bool operator==(const UnemployedElem &a, const UnemployedElem &b)
{
    bool res = ((strcmp(a.ID,b.ID) == 0) && (a.age == b.age) && (a.gender == b.gender) && (a.salary == b.salary)
                && (a.full_seniority.year == b.full_seniority.year) && (a.full_seniority.month == b.full_seniority.month)
                && (a.last_seniority.year == b.last_seniority.year) && (a.last_seniority.month == b.last_seniority.month));
    return res;
}


bool operator!=(const UnemployedElem &a, const UnemployedElem &b)
{
    return !(a == b);
}

UnemployedElem::UnemployedElem(const UnemployedElem &other) noexcept
    : Object(), age(other.age), gender(other.gender), last_seniority(other.last_seniority), full_seniority(other.full_seniority), salary(other.salary)
{
    std::copy(other.ID, other.ID + strlen(other.ID), ID);
}

UnemployedElem::UnemployedElem(UnemployedElem &&other) noexcept
    :   Object(), age(other.age), gender(other.gender), last_seniority(other.last_seniority), full_seniority(other.full_seniority), salary(other.salary)
{
    std::move(other.ID,other.ID + strlen(other.ID), ID);
    other.age = 0;
    other.gender = 0;
    other.last_seniority = {0,0};
    other.full_seniority = {0,0};
    other.salary = 0;
}
