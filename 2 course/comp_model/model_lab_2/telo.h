#ifndef TELO_H
#define TELO_H
#include <vector>
class telo
{
public:
    int left_temperature;
    int right_temperature;
    int env_temperature;

    std::vector<double> current_temperature, next_temperature;

    telo(int l, int e, int r) : left_temperature(l), right_temperature(r), env_temperature(e)
    {
      current_temperature.resize(8);
      current_temperature.assign(8,env_temperature);
      current_temperature[0] = left_temperature;
      current_temperature[7] = right_temperature;
      next_temperature = current_temperature;
    }

    void work ();
};

#endif // TELO_H
