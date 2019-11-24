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

direction animal::getStartDirection()
{
    double chance = rand_range(mt);

    if (chance < 0.25 || fabs(chance - 0.25) < 0.001) // влево
    { return direction::left; }
    else if (((chance > 0.25) && (chance < 0.50)) || (fabs(chance - 0.50) < 0.001)) // вправо
    { return direction::right; }
    else if (((chance > 0.50) && (chance < 0.75)) || (fabs(chance - 0.75) < 0.001)) // вправо
    { return direction::forward; } // прямо
    else
    { return direction::down; }
}

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
    global_direction = getStartDirection(); // начальный поворот
    for (int i = 0; i < steps; ++i)
    {
        global_direction += getDirection();

        QPoint new_point = coord.back();

        if (global_direction == direction::forward) // если крокодил идет на север
        {
            if (current_direction == direction::forward) { new_point = {x, int( y - (l*k) )}; } // Умножаю l на 10, чтобы увеличить масштаб в пикселях
            else if (current_direction == direction::left) { new_point = {int( x - (l*k) ), y}; }
            else if (current_direction == direction::right) { new_point = {int( x + (l*k) ), y}; }
        } else if (global_direction == direction::left) // запад
        {
            if (current_direction == direction::forward) { new_point = {int( x - (l*k) ), y}; }
            else if (current_direction == direction::left) { new_point = {x, int( y + (l*k) )}; }
            else if (current_direction == direction::right) { new_point = {x, int( y - (l*k) )}; }
        } else if (global_direction == direction::right) // восток
        {
            if (current_direction == direction::forward) { new_point = {int( x + (l*k) ), y}; }
            else if (current_direction == direction::left) { new_point = {x, int( y - (l*k) )}; }
            else if (current_direction == direction::right) { new_point = {x, int( y + (l*k) )}; }
        } else if (global_direction == direction::down) // юг
        {
            if (current_direction == direction::forward) { new_point = {x, int( y + (l*k) )}; }
            else if (current_direction == direction::left) { new_point = {int( x +(l*k) ), y}; }
            else if (current_direction == direction::right) { new_point = {int( x - (l*k) ), y}; }
        }

        coord.push_back(new_point);
    }
}


