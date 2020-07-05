#include "summatorlogic.h"
#include <tuple>

using namespace std;

SummatorLogic::HalfBitRes SummatorLogic::calculateHalfBitFull(bool x, bool y)
{
    bool oper1 = !x && y;
    bool oper2 = x && !y;
    bool v = oper1 || oper2;
    bool u = x && y;
    return {u,v,oper1,oper2};
}

pair<bool,bool> SummatorLogic::calculateHalfBit(bool x, bool y)
{
    bool oper1 = !x && y;
    bool oper2 = x && !y;
    bool v = oper1 || oper2;
    bool u = x && y;
    return {u,v};
}


SummatorLogic::TwoBitRes SummatorLogic::calculateTwoBit(bool a, bool b, bool c, bool d)
{
    pair<bool,bool> sum1 = calculateHalfBit(a,c);
    pair<bool,bool> sum2 = calculateHalfBit(b,d);
    pair<bool,bool> sum3 = calculateHalfBit(sum1.second,sum2.first);
    bool e = sum1.first || sum3.first;
    bool f = sum3.second;
    bool g = sum2.second;
    return {e,f,g,sum1,sum2,sum3};
}
