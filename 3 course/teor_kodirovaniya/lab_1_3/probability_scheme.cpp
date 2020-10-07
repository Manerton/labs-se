#include "probability_scheme.h"
#include "combination.h"

const std::vector<probability_scheme::scheme_value>& probability_scheme::getData() const
{
    return data;
}

double probability_scheme::getEntropy() const
{
    return entropy;
}

uint64_t probability_scheme::combination(probability_scheme::value_type k, probability_scheme::value_type n)
{
    return Combination::calculate(k,n);
}

void probability_scheme::calculate_data()
{
    if (k == 0)                 // обрабатываем случай, когда k = 0 (т.е нет стандартных изделий)
    {
        data[0] = {0,1};        // (т.е вероятность, что среди отобранных изделий нет стандартных равна 1)
    }
    else if (k == n)            // обрабатываем случай, когда k = n, т.е все изделия стандартные
    {
        auto N = data.size();
        for (size_t i = 0; i < N; ++i)
        {
            data[i].first = i;
            data[i].second = 0;
        }
        data[N-1].first = N-1;
        data[N-1].second = 1;
    }
    else // считаем если k в диапазоне (0;n)
    {
        size_t N = data.size();
        for (size_t i = 0; i < N; ++i)
        {
            data[i].first = i;

            if (m-i <= size_t(n-k))
            {
                data[i].second = double(combination(i,k) * combination(m-i,n-k)) / double(combination(m,n));
            }
            else data[i].second = 0;
        }
    }
}

void probability_scheme::calculate_entropy()
{
    if (k != 0 && k != n) // при k = 0 и k = n - энтропия равна нулю
    {
        for (const auto &var : data)
        {
            if (var.second != 0)
            {
                entropy += var.second * log2(var.second);
            }
        }
        entropy = -entropy;
    }
}
