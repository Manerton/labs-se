#ifndef THIRTEEN_H
#define THIRTEEN_H

#include "number.h"
#include <cinttypes>


class Thirteen : public Number
{
    // переводим введенные буквы a, b и с в 10, 11 и 12 соответственно
    // это будем делать перед вызовом базового конструктора
    value_type letterToNumSys(value_type t) const;    // для одного числа
    Number letterInInitListToNumSys(const std::initializer_list<Array::value_type> &t) const;  // для init list
    Number letterInStrToNumSys(const std::string &t) const;    // для строки
public:
    // мультиметоды
    virtual std::string MultMet_dynCast(const Number &b);
    virtual std::string MultMet_typeId(const Number &b);
    virtual std::string MultMet_virtFunc(const Number &b) const;
    virtual std::string MultMet_virtFunc(const Binary &b) const;
    virtual std::string MultMet_virtFunc(const Thirteen &b) const;
    virtual std::string MultMet_virtFunc(const Seven &b) const;
    std::string toString() const noexcept;  // вывод (буквы выводим, вместо 10, 11 и так далее)
    // -- конструкторы и присваивание -- //
    // указываем систему счисления и проверяем, чтобы числа ей соответстовали
    Thirteen() : Number() {}
    Thirteen(const size_type& n, value_type t = 0) : Number(n,letterToNumSys(t))
    {
        num_sys = 13;
        CheckForNumSys(*this);
    }
    Thirteen(const std::initializer_list<value_type> &t) : Number(letterInInitListToNumSys(t))
    {
        num_sys = 13;
        CheckForNumSys(*this);
    }
    Thirteen(const std::string &str) : Number(letterInStrToNumSys(str))
    {
        num_sys = 13;
        CheckForNumSys(*this);
    }
    // чтобы можно было присвоить значение типа Number (который возвращается при арифм. операциях - друж. функциях)
    // move - чтобы работал конструктор перемещения
    Thirteen(const Number& other) : Number(other) { }
    Thirteen(Number&& other) : Number(std::move(other)) {}
};


#endif // THIRTEEN_H
