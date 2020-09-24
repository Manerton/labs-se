#include "probability_scheme.h"

const std::vector<probability_scheme::scheme_value>& probability_scheme::getData() const
{
    return data;
}

double probability_scheme::getEntropy() const
{
    return entropy;
}

uint32_t probability_scheme::combination(uint16_t k, uint16_t n)
{
    if (n == k) return 1;
    if (k == 0) return 1;
    if (k == 1) return n;
    return combination(k-1, n-1) + combination(k, n-1); // теорема о сумме числа сочетаний
}

void probability_scheme::calculate_data()
{
    if (p == 0) // обрабатываем случай когда p = 0
    {
        data[0].first = 0;
        data[0].second = 1;
        for (auto k = 1; k < n+1; ++k)
        {
            data[k].first = k;
            data[k].second = 0;
        }
    }
    else if (p == 1) // обрабатываем случай когда p = 1
    {
        for (auto k = 0; k < n; ++k)
        {
            data[k].first = k;
            data[k].second = 0;
        }
        data[n].first = n;
        data[n].second = 1;
    }
    else // считаем если p в диапазоне (0;1)
    {
        double mnoz1 = 1;           // p^k
        double mnoz2 = pow(1-p, n); // q^(n-k)
        for (auto k = 0; k < n+1; ++k)
        {
            data[k].first = k;
            data[k].second = combination(k,n) * mnoz1 * mnoz2;
            mnoz1 *= p;
            mnoz2 /= (1-p);
        }
    }
}

void probability_scheme::calculate_entropy()
{
    if (p != 0 && p != 1) // при p = 0 и p = 1 - энтропия равна нулю
    {
        for (const auto &var : data)
        {
            entropy += var.second * log2(var.second);
        }
        entropy = -entropy;
    }
}
