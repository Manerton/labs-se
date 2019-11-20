#include "bitstring.h"

using namespace std;

bool BitString::CheckForBinary(const string &str) const noexcept // проверка содержимого битовой строки, чтобы оно было двоичным
{
    uint8_t len = static_cast<uint8_t>(str.length()); // uint8_t, так как у нас длина строки не больше 128-бит
    for (uint8_t i = 0; i < len; ++i)
    {
        if (str[i] != '0' && str[i] != '1') return false;
    }
    return true;
}

void BitString::CallCheck(const string &str) const // проверка содержимого битовой строки, чтобы оно было двоичным и не больше 128 бит
{
    if (str.length() > 128) throw overflow_exception();
    if (!CheckForBinary(str)) throw not_binary_exception();
}

uint64_t BitString::BinaryString_toInt(const string &str) const noexcept // перевожу битовую строку в 64-битное десятичное число
{
    string temp = str;
    reverse(temp.begin(),temp.end()); // переворачиваю строку, чтобы идти с конца

    uint64_t res = 0, m = 1;
    uint8_t len = static_cast<uint8_t>(str.length());
    for (uint8_t i = 0; i < len; ++i)
    {
        if (temp[i] == '1')
        {
            res += m << i; // прибавляю к res число 2^i
        }
    }
    return res;
}

string BitString::toString() const noexcept // перевожу десятичные числа из полей в строку с двоичным кодом
{
    stringstream ss;
    uint64_t m = 1;
    for (int i = 0; i < 64; ++i)
    {
        ss << ((F2 & (m << i)) ? 1 : 0); // иду с конца числа F2 и пишу 1 или 0 в зависимости от бита числа на позиции i
    }
    for (int i = 0; i < 64; ++i)
    {
        ss << ((F1 & (m << i)) ? 1 : 0);
    }
    string s = ss.str();
    reverse(s.begin(),s.end()); // Так как шел с конца, а записывал в stringstream с начала, то переворачиваю строку
    return s;
}

string BitString::GetOptimizedBinaryString() const noexcept // отбрасываю незначащие нули слева при выводе строки и для функции проверки включения
{
    string s = toString();
    // ищем первый элемент равный 1
    auto pos = find_if(s.begin(), s.end(),
                        [](char elem){
                           return (elem == '1');
                        });

    if (pos != s.begin()) // если первый элемент не равен 1
    {
        if (pos == s.end()) s = "0"; // если единица не найдена
        else s.erase(s.begin(), pos); // удаляем лишние нули
    }

    return s;

}

int BitString::count_of_SingleBit() const noexcept // количество битовый единиц в строке, возвращаю int вместо uint8_t, так как хочу число, а не символ
{
    string str = toString();
    uint8_t res = static_cast<uint8_t>(count(str.begin(), str.end(), '1'));
    return res;
}

void BitString::SetOutputFlag() noexcept
{
    OutputFlag = true;
}

void BitString::ClearOutputFlag() noexcept
{
    OutputFlag = false;
}

// *** операции-методы ***

BitString& BitString::operator&=(const BitString &b)
{
    F1 &= b.F1;
    F2 &= b.F2;
    return *this;
}

BitString& BitString::operator|=(const BitString &b)
{
    F1 |= b.F1;
    F2 |= b.F2;
    return *this;
}

BitString& BitString::operator^=(const BitString &b)
{
    F1 ^= b.F1;
    F2 ^= b.F2;
    return *this;
}

BitString& BitString::operator<<=(const uint8_t &i)
{
    string str = toString();
    uint8_t len = static_cast<uint8_t>(str.length());
    string strnew;
    if (len+i > 128)
    {
        strnew.assign(len,'0');
        strnew.replace(0,len-i,str.substr(i,len));
    } else {
        strnew.assign(len+i,'0');
        strnew.replace(0,len,str);
    }
    *this = BitString(strnew); // создаю локальный BitString от строки strnew
    return *this;
}

BitString& BitString::operator>>=(const uint8_t &i)
{
    string str = toString();
    uint8_t len = static_cast<uint8_t>(str.length());
    string strnew;
    if (len+i > 128)
    {
        strnew.assign(len,'0');
        strnew.replace(i,len-i,str.substr(0,len-i));
    } else {
        strnew.assign(len+i,'0');
        strnew.replace(i,len,str);
    }
    *this = BitString(strnew);
    return *this;
}

BitString BitString::operator~() // побитовое not
{
    BitString t = *this;
    t.F1 = ~F1;
    t.F2 = ~F2;
    return t;
}

BitString BitString::operator<<(const uint8_t &i) // побитовый сдвиг влево
{
    BitString t = *this;
    t <<= i;
    return t;
}

BitString BitString::operator>>(const uint8_t &i) // побитовый сдвиг вправо
{
    BitString t = *this;
    t >>= i;
    return t;
}

// *** операторы - дружественные функции ***
BitString operator&(const BitString &a, const BitString &b) // побитовое and
{
    BitString t = a;
    t &= b;
    return t;
}

BitString operator|(const BitString &a, const BitString &b) // побитовое or
{
    BitString t = a;
    t |= b;
    return t;
}

BitString operator^(const BitString &a, const BitString &b) // побитовое xor
{
    BitString t = a;
    t ^= b;
    return t;
}

// *** операторы сравнения - дружественные функции ***
bool operator==(const BitString &a, const BitString &b)
{
    return (a.count_of_SingleBit() == b.count_of_SingleBit());
}
bool operator!=(const BitString &a, const BitString &b)
{
    return !(a == b);
}
bool operator<(const BitString &a, const BitString &b)
{
    return (a.count_of_SingleBit() < b.count_of_SingleBit());
}
bool operator>=(const BitString &a, const BitString &b)
{
    return !(a<b);
}

bool operator>(const BitString &a, const BitString &b)
{
    return (b < a); // так как a > b, это b < a
}

bool operator<=(const BitString &a, const BitString &b)
{
    return !(b<a); // тоже реализую через одну операцию <
}

bool is_included(const BitString &a, const BitString &b) // включен ли a в b
{
    string s1, s2;
    s1 = a.GetOptimizedBinaryString();
    s2 = b.GetOptimizedBinaryString();
    return (s2.find(s1) != string::npos);
}

ostream& operator<<(ostream& t, const BitString &r)
{
    string s;
    if (r.OutputFlag) s = r.GetOptimizedBinaryString();
    else s = r.toString();
    return (t << s);
}
istream& operator>>(istream& t, BitString &r)
{
    string str;
    t >> str;
    r = str;
    return t;
}
