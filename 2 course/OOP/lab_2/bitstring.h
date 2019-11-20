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
    BitString() : F1(0), F2(0) {}
    BitString(uint64_t chislo) : F1(0), F2(chislo){} // конструктор с одним целым
    BitString(uint64_t _F1, uint64_t _F2) : F1(_F1), F2(_F2){} // с двумя целыми
    BitString(const std::string &str) : F1(0), F2(0) // со строкой
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
    std::string toString() const noexcept; // перевод двух десятичных 64-битных чисел в двоичную строку
    int count_of_SingleBit() const noexcept; // количество битовый единиц в строке
    void SetOutputFlag() noexcept;
    void ClearOutputFlag() noexcept;
    // операции-методы
    BitString& operator&=(const BitString &b);
    BitString& operator|=(const BitString &b);
    BitString& operator^=(const BitString &b);
    BitString& operator<<=(const uint8_t &i);
    BitString& operator>>=(const uint8_t &i);
    BitString operator~();
    BitString operator<<(const uint8_t &i);
    BitString operator>>(const uint8_t &i);
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
