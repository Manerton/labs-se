#include "animal.h"
#include <ctime>
#include <random>

using namespace std;

//Глобальные переменные для рандома
static time_t seed = time(nullptr);
static mt19937 mt(seed);
static uniform_real_distribution<double> rand_range(0,1); // от 0 до 1

//direction animal::getDirection()
//{
//    double chance = rand_range(mt);

//    if (chance < 0.25 || fabs(chance - 0.25) < 0.001) // влево
//    { return direction::left; }
//    else if (((chance > 0.25) && (chance < 0.5)) || (fabs(chance - 0.5) < 0.001)) // вправо
//    { return direction::right; }
//    else // прямо
//    { return direction::forward; }
//}


direction animal::getDirection()
{
    double chance = rand_range(mt);

    if (chance < 0.33 || fabs(chance - 0.33) < 0.001) // влево
    { return direction::left; }
    else if (((chance > 0.33) && (chance < 0.66)) || (fabs(chance - 0.66) < 0.001)) // вправо
    { return direction::right; }
    else // прямо
    { return direction::forward; }
}

void animal::work(int time, int k)
{
    coord.clear();
    int steps = int(v * time / l); // количество поворотов крокодила, каждые l км его пути
    coord.push_back(QPoint(0,0)); // начало пути
    prev_direction = getDirection(); // начальный поворот
    for (int i = 0; i < steps; ++i)
    {
        direction current_direction = getDirection();
        int x = coord.back().x(); // получаем координату x предыдущей точки
        int y = coord.back().y(); // получаем координату y предыдущей точки
        QPoint new_point;
        if (prev_direction == direction::forward) // если до этого шли прямо
        {
            if (current_direction == direction::forward) { new_point = {x, int( y - (l*k) )}; } // Умножаю l на 10, чтобы увеличить масштаб в пикселях
            else if (current_direction == direction::left) { new_point = {int( x - (l*k) ), y}; }
            else if (current_direction == direction::right) { new_point = {int( x + (l*k) ), y}; }
        } else if (prev_direction == direction::left) // если до этого шли влево
        {
            if (current_direction == direction::forward) { new_point = {int( x - (l*k) ), y}; }
            else if (current_direction == direction::left) { new_point = {x, int( y + (l*k) )}; }
            else if (current_direction == direction::right) { new_point = {x, int( y - (l*k) )}; }
        } else if (prev_direction == direction::right) // если до этого шли право
        {
            if (current_direction == direction::forward) { new_point = {int( x + (l*k) ), y}; }
            else if (current_direction == direction::left) { new_point = {x, int( y - (l*k) )}; }
            else if (current_direction == direction::right) { new_point = {x, int( y + (l*k) )}; }
        }
        coord.push_back(new_point);
        prev_direction = current_direction;
    }
}


