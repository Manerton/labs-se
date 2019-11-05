#include "telo.h"

void telo::work()
{
    next_temperature = current_temperature;
    for (size_t j = 1; j < 7; ++j)
    {
        next_temperature[j] = (current_temperature[j-1] + current_temperature[j+1]) / 2;
    }
    current_temperature = next_temperature;
}


