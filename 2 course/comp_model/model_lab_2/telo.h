#ifndef TELO_H
#define TELO_H
#include <vector>
#include <cinttypes>
class telo
{
    double l; // длина стержня
    std::vector<int16_t> current_temperature, next_temperature;
    int16_t left_temperature = 100;
    int16_t right_temperature = 50;
    int16_t env_temperature = 20;
public:
    telo(uint64\&inl) : l(inl)
    {
      current_temperature.resize(8);
      next_temperature.resize(8);
      current_temperature.assign(8,env_temperature);
      current_temperature[0] = left_temperature;
      current_temperature[7] = right_temperature;
    }
    double getAngle (const double &gradus);
    QString work (double a);
};

#endif // TELO_H
