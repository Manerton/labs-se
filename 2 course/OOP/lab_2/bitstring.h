#ifndef BITSTRING_H
#define BITSTRING_H

#include <iostream>
#include <sstream>
#include <string>

class BitString
{
    uint64_t F1, F2;
    bool Check(const std::string str) const;
    class negative_int {};
    class not_binary {};
    class overflow {};

public:
    BitString() : F1(0), F2(0) {}
    std::string toString() const noexcept;
    friend uint64_t fromString(const std::string &str) noexcept;
    friend std::ostream& operator<<(std::ostream& t, const BitString &r);
    friend std::istream& operator>>(std::istream& t, BitString &r);
};

#endif // BITSTRING_H
