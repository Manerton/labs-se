#include "binary.h"
#include "thirteen.h"
#include "number.h"
#include "seven.h"
#include <exception>
#include <iostream>

using namespace std;

std::string Binary::MultMet_dynCast(const Number &b)
{
    if ([[maybe_unused]] const Binary *pb = dynamic_cast<const Binary*>(&b))
    {
        // правый аргумент типа Binary; обработка варианта "Binary-Binary"
        return {"Binary-Binary"};
    }
    else if ([[maybe_unused]] const Thirteen *pb = dynamic_cast<const Thirteen*>(&b))
    {
        // правый аргумент типа Thirteen; обработка варианта "Binary-Thirteen"
        return {"Binary-Thirteen"};
    }
    else if ([[maybe_unused]] const Seven *pb = dynamic_cast<const Seven*>(&b))
    {
        // правый аргумент типа Seven; обработка варианта "Binary-Seven"
        return {"Binary-Seven"};
    }
    else throw std::logic_error("Error! Incorrect type argument!");
}

string Binary::MultMet_typeId(const Number &b)
{
    if (typeid(Binary) == typeid(b))
    {
        return {"Binary-Binary"};
    }
    else if (typeid(Thirteen) == typeid(b))
    {
        return {"Binary-Thirteen"};
    }
    else if (typeid(Seven) == typeid(b))
    {
        return {"Binary-Seven"};
    }
    else throw std::logic_error("Error! Incorrect type argument!");
}

string Binary::MultMet_virtFunc(const Number &b) const
{
    return b.MultMet_virtFunc(*this);
}

string Binary::MultMet_virtFunc(const Binary &b) const
{
    return {"Binary-Binary"};
}

string Binary::MultMet_virtFunc(const Thirteen &b) const
{
    return {"Binary-Thirteen"};
}

string Binary::MultMet_virtFunc(const Seven &b) const
{
    return {"Binary-Seven"};
}
