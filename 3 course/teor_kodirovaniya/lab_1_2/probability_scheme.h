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
    double entropy = 0;
    std::vector<scheme_value> data; // значения в вер. схеме
    void calculate_data();
    void calculate_entropy();
public:
    probability_scheme() = delete;
    probability_scheme(double _p) : p(_p)
    {
        calculate_data();
        calculate_entropy();
    }
    const std::vector<scheme_value>& getData() const;
    double getEntropy() const;
};

#endif // PROBABILITY_SCHEME_H
