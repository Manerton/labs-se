#ifndef BITSTRING_H
#define BITSTRING_H

class BitString
{
    unsigned long long F1, F2;
    bool Check(const long long f1_in, const long long f2_in) const;
public:
    BitString() : F1(0), F2(0) {};
};

#endif // BITSTRING_H
