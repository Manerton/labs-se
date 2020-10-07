#ifndef PROBABILITY_SCHEME_H
#define PROBABILITY_SCHEME_H
#include <cstdint>
#include <utility>
#include <vector>
#include <cmath>
#include <QDebug>

class probability_scheme
{
public:
    using value_type = uint16_t;
    using scheme_value = std::pair<value_type,double>;  // пара Xi - Pi в вероятностной схеме
private:
    value_type n = 0;                                   // общее количество изделий
    value_type k = 0;                                   // количество стандартных изделий
    value_type m = 0;                                   // количество отобранных изделий
    double entropy = 0;                                 // значение энтропии для заданного k
    std::vector<scheme_value> data;                     // значения в вер. схеме
    uint64_t combination(value_type k, value_type n);   // вычислить сочетания из n по k
    void calculate_data();                              // рассчитываем схему
    void calculate_entropy();                           // рассчитываем энтропию
public:
    probability_scheme() = delete;
    probability_scheme(value_type _n, value_type _k, value_type _m) : n(_n), k(_k), m(_m)
    {
        value_type N = std::min(m,k);
        data.resize(N+1);
        calculate_data();
        calculate_entropy();
    }

    const std::vector<scheme_value>& getData() const;
    double getEntropy() const;
};

#endif // PROBABILITY_SCHEME_H
