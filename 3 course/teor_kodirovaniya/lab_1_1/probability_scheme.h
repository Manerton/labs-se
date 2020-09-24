#ifndef PROBABILITY_SCHEME_H
#define PROBABILITY_SCHEME_H
#include <cstdint>
#include <utility>
#include <vector>
#include <cmath>
#include <QDebug>

class probability_scheme
{
    using scheme_value = std::pair<uint16_t,double>; // пара Xi - Pi в вероятностной схеме
    double p = 0;   // вероятность события A
    uint16_t n = 0; // количество независимых испытаний
    double entropy = 0;
    std::vector<scheme_value> data; // значения в вер. схеме
    uint32_t combination(uint16_t k, uint16_t n); // вычислить сочетания из n по k
    void calculate_data();
    void calculate_entropy();
public:
    probability_scheme() = delete;
    probability_scheme(double _p, uint16_t _n) : p(_p), n(_n)
    {
        data.resize(n+1); // так как в схеме от 0 до n (а не от 0 до n-1)
        calculate_data();
        calculate_entropy();
    }
    const std::vector<scheme_value>& getData() const;
    double getEntropy() const;
};

#endif // PROBABILITY_SCHEME_H
