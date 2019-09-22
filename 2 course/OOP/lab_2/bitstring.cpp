#include "bitstring.h"
#include <stdint.h>


bool BitString::Check(const std::string str) const // проверка числа, чтобы оно было двоичным и неотрицательным
{
    if (str.length() > 128) throw overflow();
    if (str.find('-') == std::string::npos)
    {
        uint8_t len = static_cast<uint8_t>(str.length());
        for (uint8_t i = 0; i < len; i++)
        {
            if (str[i] != '0' && str[i] != '1') throw not_binary();
        }
    } else {throw negative_int();}

    return true;
}

std::string BitString::toString() const noexcept
{
    std::stringstream ss;
    uint64_t m = 1;
    for (int i = 64; i > 0; --i)
    {
        ss << ((F1 & (m << (i-1))) ? 1 : 0);
    }
    for (int i = 64; i > 0; --i)
    {
        ss << ((F2 & (m << (i-1))) ? 1 : 0);
    }
    return ss.str();
}

uint64_t fromString(const std::string &str) noexcept // из строки с двоичным кодом в число uint_64
{
    uint64_t res = 0;
    uint64_t m = 1;
    uint8_t len = static_cast<uint8_t>(str.length());
    for(uint8_t i = 0; i < len; i++)
    {
        res += (str[i] == '1') ? (m << (len - i - 1)) : 0;
    }
    return res;
}

std::ostream& operator<<(std::ostream& t, const BitString &r)
{
    std::string s = r.toString();
    return (t << s);
}
std::istream& operator>>(std::istream& t, BitString &r)
{
    std::string str;
    t >> str;
    if (r.Check(str))
    {
        uint8_t len = static_cast<uint8_t>(str.length());
        if (len <= 64) r.F2 = fromString(str);
        else {
            std::string temp(128,'0');
            temp.replace(128-len,len,str);
            r.F1 = fromString(temp.substr(0,64));
            r.F2 = fromString(temp.substr(64,128));
        }
    }
    return t;
}
