#include "animal.h"
#include <ctime>
#include <random>

using namespace std;

//Глобальные переменные для рандома
static time_t seed = time(nullptr);
static mt19937 mt(seed);
static uniform_real_distribution<double> rand_range(0,1); // от 0 до 1

direction animal::getStartDirection()
{
    double chance = rand_range(mt);

    if (chance < 0.25 || fabs(chance - 0.25) < 0.001) // влево
    { return direction::left; }
    else if (((chance > 0.25) && (chance < 0.50)) || (fabs(chance - 0.50) < 0.001)) // вправо
    { return direction::right; }
    else if (((chance > 0.50) && (chance < 0.75)) || (fabs(chance - 0.75) < 0.001)) // прямо
    { return direction::forward; }
    else
    { return direction::down; }
}

direction& operator+=(direction &a, const direction &b)
{
    int _a = static_cast<int>(a);
    int _b = static_cast<int>(b);
    _a += _b;
    _a %= 4;
    a = static_cast<direction>(_a);
    return a;

}

direction animal::getRotation(double veroyatnost)
{
    double chance = rand_range(mt);

    if (chance < veroyatnost || fabs(chance - veroyatnost) < 0.001) // влево
    { return direction::left; }
    else if (((chance > veroyatnost) && (chance < veroyatnost * 2)) || (fabs(chance - veroyatnost * 2) < 0.001)) // вправо
    { return direction::right; }
    else // прямо
    { return direction::forward; }
}



void animal::work(int time, int k, double veroyatnost)
{
    coord.clear();
    int steps = int(v * time / l); // количество поворотов крокодила, каждые l км его пути
    coord.push_back(QPoint(0,0)); // начало пути
    global_direction = getStartDirection(); // начальный поворот
    for (int i = 0; i < steps; ++i)
    {

        global_direction += getRotation(veroyatnost);
        QPoint new_point = coord.back();

        if (global_direction == direction::forward) // если крокодил идет на север
        {
            new_point.ry() -= int(l * k);
        } else if (global_direction == direction::left) // запад
        {
            new_point.rx() -= int(l * k);
        } else if (global_direction == direction::right) // восток
        {
            new_point.rx() += int(l * k);
        } else if (global_direction == direction::down) // юг
        {
            new_point.ry() += int(l * k);
        }

        coord.push_back(new_point);
    }
}


