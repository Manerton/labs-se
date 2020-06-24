#include "thirteen.h"
#include "binary.h"
#include "seven.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>

using namespace std;

Number::value_type Thirteen::letterToNumSys(Array::value_type t) const
{
    if (t == 'a' || t == 'A' || t == 'a'-'0' || t == 'A'-'0')
    {
        t = 10;
    }
    else if (t == 'b' || t == 'B' || t == 'b'-'0' || t == 'B'-'0')
    {
        t = 11;
    }
    else if (t == 'c' || t == 'C' || t == 'c'-'0' || t == 'C'-'0')
    {
        t = 12;
    }
    return t;
}

Number Thirteen::letterInInitListToNumSys(const std::initializer_list<Array::value_type> &t) const
{
    Number arr(t);
    for (value_type &v : arr)
    {
        v = letterToNumSys(v);
    }
    return arr;
}

Number Thirteen::letterInStrToNumSys(const string &t) const
{
    Number arr(t);
    for (value_type &v : arr)
    {
        v = letterToNumSys(v);
    }
    return arr;
}

std::string Thirteen::MultMet_dynCast(const Number &b)
{
    if (const Binary *pb = dynamic_cast<const Binary*>(&b))
    {
        // правый аргумент типа Binary; обработка варианта "Thirteen-Binary"
        return {"Thirteen-Binary"};
    }
    else if (const Thirteen *pb = dynamic_cast<const Thirteen*>(&b))
    {
        // правый аргумент типа Thirteen; обработка варианта "Thirteen-Thirteen"
        return {"Thirteen-Thirteen"};
    }
    else if (const Seven *pb = dynamic_cast<const Seven*>(&b))
    {
        // правый аргумент типа Seven; обработка варианта "Thirteen-Seven"
        return {"Thirteen-Seven"};
    }
    else throw std::logic_error("Error! Incorrect type argument!");
}

string Thirteen::MultMet_typeId(const Number &b)
{
    if (typeid(Binary) == typeid(b))
    {
        return {"Thirteen-Binary"};
    }
    else if (typeid(Thirteen) == typeid(b))
    {
        return {"Thirteen-Thirteen"};
    }
    else if (typeid(Seven) == typeid(b))
    {
        return {"Thirteen-Seven"};
    }
    else throw std::logic_error("Error! Incorrect type argument!");
}

string Thirteen::MultMet_virtFunc(const Number &b) const
{
    return b.MultMet_virtFunc(*this);
}

string Thirteen::MultMet_virtFunc(const Binary &b) const
{
    return {"Thirteen-Binary"};
}

string Thirteen::MultMet_virtFunc(const Thirteen &b) const
{
    return {"Thirteen-Thirteen"};
}

string Thirteen::MultMet_virtFunc(const Seven &b) const
{
    return {"Thirteen-Seven"};
}


string Thirteen::toString() const noexcept
{
    if (get_size() != 0)
    {
        std::stringstream ss;
        for (value_type val : (*this))
        {
            if (val == 10)
            {
                ss << "A" << " ";
            }
            else if (val == 11)
            {
                ss << "B" << " ";
            }
            else if (val == 12)
            {
                 ss << "C" << " ";
            }
            else
            {
                ss << int(val) << " ";
            }
        }
        std::string s(ss.str());
        s.pop_back(); // -- удаляю лишний пробел -- //
        return s;
    }
    return "empty";
}
