#include "probability_scheme.h"

const std::vector<probability_scheme::scheme_value>& probability_scheme::getData() const
{
    return data;
}

double probability_scheme::getEntropy() const
{
    return entropy;
}

void probability_scheme::calculate_data()
{
    if (p == 1) // обрабатываем случай когда p = 1
    {
        data.push_back({1,1});
    }
    else // считаем если p в диапазоне (0;1)
    {
        double mnoz = 1; // q^(k-1), 1 так как k = 1
        double sum_Pi = 0;
        const double eps = 0.01;
        int k = 1;
        while (fabs(sum_Pi-1) > eps)
        {
            double Pi = p*mnoz;
            data.push_back({k,(Pi)});
            mnoz *= (1-p);
            ++k;
            sum_Pi += Pi;
        }
    }
}

void probability_scheme::calculate_entropy()
{
    if (p != 1) // при p = 1 - энтропия равна нулю
    {
        for (const auto &var : data)
        {
            entropy += var.second * log2(var.second);
        }
        entropy = -entropy;
    }
}
