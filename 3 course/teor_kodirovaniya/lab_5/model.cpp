#include <sstream>
#include <cmath>
#include <bitset>
#include <algorithm>
#include <QDebug>
#include <random>
#include "model.h"

using namespace std;

static mt19937 mt(time(0));

int Model::getR() const
{
    return r;
}

int Model::getN() const
{
    return n;
}

BigBinary Model::getB() const
{
    return B;
}

BigBinary Model::getB_with_err() const
{
    return B_with_err;
}

int Model::getError_razryad() const
{
    return error_razryad;
}

BigBinary Model::getGen_polynom() const
{
    return gen_polynom;
}

BigBinary Model::getKodirovanie1() const
{
    return kodirovanie1;
}

BigBinary Model::getKodirovanie2() const
{
    return kodirovanie2;
}

int Model::getShift_count() const
{
    return shift_count;
}

BigBinary Model::getB_fixed() const
{
    return B_fixed;
}

void Model::calculate_r()
{
    double val = log2(k+1+log2(k+1));
    r = ceil(val);
    n = k+r;
}

void Model::add_error_to_B()
{
    B_with_err = B;
    uniform_int_distribution<int> error_dist(0, n-1);
    size_t error_i = error_dist(mt);
    B_with_err[error_i] ^= 1;
}

void Model::find_gen_polynom()
{
    int num = 3;
    BigBinary polynom(num);
    // третье условие, чтобы двучлен x^n+1 делился на образующий многочлен без остатка
    string binomial_str(n+1,'0');
    binomial_str.front() = '1';
    binomial_str.back() = '1';
    BigBinary binomial(binomial_str);

    while (     !(polynom.size() >= size_t(r+1))        // должен быть наименее короким, но сепень должна быть не менее r
             || !(polynom.get_W() >= size_t(d0))        // число единич. разрядов должно быть не менее кодового расстояния d0
             || !(((binomial % polynom).get_W() == 0))) // g(x) должен делить x^n + 1 без остатка
    {
        num += 2;
        polynom = BigBinary(num);
    }
    gen_polynom = polynom;
}

void Model::kodirovanie()
{
    // 1 шаг
    string kodirovanie1_str = A.toString();
    kodirovanie1_str.insert(kodirovanie1_str.end(),r,'0');
    kodirovanie1 = {kodirovanie1_str};
    // 2 шаг
    kodirovanie2 = kodirovanie1 % gen_polynom;
    // 3 шаг
    B = kodirovanie1 + kodirovanie2;
}

void Model::find_error()
{
    B_fixed = B_with_err;
    BigBinary ostatok = B_fixed % gen_polynom;
    shift_count = 0;
    while (ostatok.get_W() > size_t(t))
    {
        B_fixed <<= 1;
        ++shift_count;
        ostatok = B_fixed % gen_polynom;
    }
    B_fixed += ostatok;
    B_fixed >>= shift_count;
    if (B_fixed != B) B_fixed = 0;
}
