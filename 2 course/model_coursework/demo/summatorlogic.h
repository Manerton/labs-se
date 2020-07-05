#ifndef SUMMATORLOGIC_H
#define SUMMATORLOGIC_H
#include <tuple>

class SummatorLogic
{
public:
    struct HalfBitRes
    {
        bool u;
        bool v;
        bool oper1;
        bool oper2;
    };
    struct TwoBitRes
    {
        bool e;
        bool f;
        bool g;
        std::pair<bool,bool> sum1;
        std::pair<bool,bool> sum2;
        std::pair<bool,bool> sum3;
    };
    SummatorLogic() = default;
    // полный результат, u, v, oper1, oper2
    HalfBitRes calculateHalfBitFull(bool x, bool y);
    // только u и v
    std::pair<bool,bool> calculateHalfBit(bool x, bool y);
    // возвращает e, f, g, sum1, sum2, sum3
    TwoBitRes calculateTwoBit(bool a, bool b, bool c, bool d);
};

#endif // SUMMATORLOGIC_H
