#include "binary.h"
#include "thirteen.h"
#include "seven.h"
#include <exception>
#include <iostream>

using namespace std;

string Seven::MultMet_dynCast(const Number &b)
{
    if (const Binary *pb = dynamic_cast<const Binary*>(&b))
    {
        // правый аргумент типа Binary; обработка варианта "Seven-Binary"
        return {"Seven-Binary"};
    }
    else if (const Thirteen *pb = dynamic_cast<const Thirteen*>(&b))
    {
        // правый аргумент типа Thirteen; обработка варианта "Seven-Thirteen"
        return {"Seven-Thirteen"};
    }
    else if (const Seven *pb = dynamic_cast<const Seven*>(&b))
    {
        // правый аргумент типа Seven; обработка варианта "Seven-Seven"
        return {"Seven-Seven"};
    }
    else throw std::logic_error("Error! Incorrect type argument!");
}

string Seven::MultMet_typeId(const Number &b)
{
    if (typeid(Binary) == typeid(b))
    {
        return {"Seven-Binary"};
    }
    else if (typeid(Thirteen) == typeid(b))
    {
        return {"Seven-Thirteen"};
    }
    else if (typeid(Seven) == typeid(b))
    {
        return {"Seven-Seven"};
    }
    else throw std::logic_error("Error! Incorrect type argument!");
}

string Seven::MultMet_virtFunc(const Number &b) const
{
    return b.MultMet_virtFunc(*this);
}

string Seven::MultMet_virtFunc(const Binary &b) const
{
    return {"Seven-Binary"};
}

string Seven::MultMet_virtFunc(const Thirteen &b) const
{
    return {"Seven-Thirteen"};
}

string Seven::MultMet_virtFunc(const Seven &b) const
{
    return {"Seven-Seven"};
}
