#ifndef BITSTRING_H
#define BITSTRING_H

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstdint>

class BitString
{
    uint64_t F1, F2;
    bool OutputFlag = true; // флаг - по умолчанию убирать незначащие нули слева при выводе
    bool CheckForBinary(const std::string &str) const noexcept; // проверка содержимого битовой строки, чтобы оно было двоичным
    void CallCheck(const std::string &str) const; // проверка содержимого битовой строки, чтобы оно было двоичным и не больше 128 бит
    uint64_t BinaryString_toInt(const std::string &str) const noexcept; // перевожу битовую строку в 64-битное десятичное число
    std::string GetOptimizedBinaryString() const noexcept; // отбрасываю незначащие нули слева при выводе строки и для функции проверки включения

public:
    BitString(uint64_t _F1 = 0, uint64_t _F2 = 0) : F1(_F1), F2(_F2) // с двумя целыми
    {
        if (_F1 != 0 && _F2 == 0) { F1 = _F2; F2 = _F1; } // -- если было введено одно число, то помещаем это число в поле F2 (в младшую правую часть) -- //
    }
    BitString(const std::string &str) : F1(0), F2(0) // со строкой
    {
        CallCheck(str);
        size_t len = str.length();
        if (len <= 64) F2 = BinaryString_toInt(str);
        else {
            std::string temp(128,'0');
            temp.replace(128-len,len,str);
            F1 = BinaryString_toInt(temp.substr(0,64));
            F2 = BinaryString_toInt(temp.substr(64,128));
        }
    }
    std::string toString() const noexcept; // перевод двух десятичных 64-битных чисел в двоичную строку
    int64_t count_of_SingleBit() const noexcept; // количество битовый единиц в строке
    void SetOutputFlag() noexcept; // -- установить (включить) флаг (т.е убирать незначащие нули слева при выводе) -- //
    void ClearOutputFlag() noexcept; // -- сбросить (выключить) этот флаг (т.е не убирать незначащие нули слева при выводе) -- //
    // операции-методы
    BitString& operator&=(const BitString &b);
    BitString& operator|=(const BitString &b);
    BitString& operator^=(const BitString &b);
    BitString& operator<<=(const uint8_t &i);
    BitString& operator>>=(const uint8_t &i);
    BitString operator~();
    BitString operator<<(const uint8_t &i); // -- побитовый сдвиг влево -- //
    BitString operator>>(const uint8_t &i); // -- побитовый сдвиг вправо -- //
    // побитовые операции
    friend BitString operator&(const BitString &a, const BitString &b);
    friend BitString operator^(const BitString &a, const BitString &b);
    friend BitString operator|(const BitString &a, const BitString &b);
    //операции сравнения
    friend bool operator==(const BitString &a, const BitString &b);
    friend bool operator!=(const BitString &a, const BitString &b);
    friend bool operator<(const BitString &a, const BitString &b);
    friend bool operator>=(const BitString &a, const BitString &b);
    friend bool operator>(const BitString &a, const BitString &b);
    friend bool operator<=(const BitString &a, const BitString &b);
    friend bool is_included(const BitString &a, const BitString &b); // операция проверки включения
    friend std::ostream& operator<<(std::ostream& t, const BitString &r); // вывод
    friend std::istream& operator>>(std::istream& t, BitString &r); //ввод
    // исключения
    class not_binary_exception {};
    class overflow_exception {};
};

#endif // BITSTRING_H
