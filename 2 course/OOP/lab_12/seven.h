#ifndef SEVEN_H
#define SEVEN_H
#include "number.h"
#include <cinttypes>

class Seven : public Number
{
public:
    // мультиметоды
    virtual std::string MultMet_dynCast(const Number &b);
    virtual std::string MultMet_typeId(const Number &b);
    virtual std::string MultMet_virtFunc(const Number &b) const;
    virtual std::string MultMet_virtFunc(const Binary &b) const;
    virtual std::string MultMet_virtFunc(const Thirteen &b) const;
    virtual std::string MultMet_virtFunc(const Seven &b) const;
    // указываем систему счисления и проверяем, чтобы числа ей соответстовали
    Seven() : Number() {}
    Seven(const size_type& n, value_type t = 0) : Number(n,t)
    {
        num_sys = 7;
        CheckForNumSys(*this);
    }
    Seven(const std::initializer_list<value_type> &t) : Number(t)
    {
        num_sys = 7;
        CheckForNumSys(*this);
    }
    Seven(const std::string &str) : Number(str)
    {
        num_sys = 7;
        CheckForNumSys(*this);
    }
    // чтобы можно было присвоить значение типа Number (который возвращается при арифм. операциях - друж. функциях)
    // move - чтобы работал конструктор перемещения
    Seven(const Number& other) : Number(other) { }
    Seven(Number&& other) : Number(std::move(other)) {}
};

#endif // SEVEN_H
