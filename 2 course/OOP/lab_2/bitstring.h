#ifndef BITSTRING_H
#define BITSTRING_H

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

class BitString
{
    uint64_t F1, F2;
    bool CheckForBinary(const std::string &str) const noexcept;
    void CallCheck(const std::string &str) const;
    uint64_t BinaryString_toInt(const std::string &str); // функция перевода из двоичных чисел в строке в десятичное 64-битное число
    std::string GetOptimizedBinary(const std::string &str) const; // отбрасываю незначащие нули слева при вывода строки
public:
    BitString() : F1(0), F2(0) {}
    BitString(const std::string &str) : F1(0), F2(0)
    {
        CallCheck(str);
        uint8_t len = static_cast<uint8_t>(str.length());
        if (len <= 64) F2 = BinaryString_toInt(str);
        else {
            std::string temp(128,'0');
            temp.replace(128-len,len,str);
            F1 = BinaryString_toInt(temp.substr(0,64));
            F2 = BinaryString_toInt(temp.substr(64,128));
        }
    }
    BitString& operator&=(const BitString &b);
    BitString& operator|=(const BitString &b);
    BitString& operator^=(const BitString &b);
    BitString operator~();
    BitString& operator<<=(const uint8_t &i);
    BitString& operator>>=(const uint8_t &i);
    BitString operator<<(const uint8_t &i);
    BitString operator>>(const uint8_t &i);
    std::string toString() const noexcept; // перевод двух десятичных 64-битных чисел в строку с двоичным числом
    int count_of_SingleBit() const noexcept;
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
    class negative_int_exception {};
    class not_binary_exception {};
    class overflow_exception {};
};

#endif // BITSTRING_H
