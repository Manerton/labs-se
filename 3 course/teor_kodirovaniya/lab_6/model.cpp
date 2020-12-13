#include <sstream>
#include <cmath>
#include <bitset>
#include <algorithm>
#include <fstream>
#include <QDebug>
#include <QMessageBox>
#include <random>
#include "model.h"

using namespace std;

static mt19937 mt(time(0));
Model::Table2 Model::table2;
Model::Table3 Model::table3;

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

void Model::setTable2(const Table2 &value)
{
    table2 = value;
}

void Model::setTable3(const Table3 &value)
{
    table3 = value;
}

int Model::getS() const
{
    return s;
}

int Model::getH() const
{
    return h;
}

int Model::getI() const
{
    return i;
}

int Model::getM() const
{
    return m;
}

void Model::calculate_r_n_s()
{
    auto it     = table2.begin();
    bool FOUND  = false;
    while (it != table2.end() && !FOUND)
    {
        if (it->k == k)
        {
            r = it->r;
            n = it->n;
            s = it->s;
            FOUND = true;
        }
        ++it;
    }
}

void Model::calculate_h_i()
{
    // чтобы получить h как индекс элемента в таблице представленной в программе
    const int h_offset = -2;
    h = log2(n+1) + h_offset;
    i = 2*s - 1;
}

void Model::add_error_to_B()
{
    B_with_err = B;
    uniform_int_distribution<int> error_count_dist(1, s);
    uniform_int_distribution<int> error_dist(0, n-1);
    m = error_count_dist(mt);

    vector<int> prev_errors;
    for (int i = 0; i < m; ++i)
    {
        size_t error_i = error_dist(mt);
        while (find(prev_errors.begin(),prev_errors.end(),error_i) != prev_errors.end())
        {
            error_i = error_dist(mt);
        }
        B_with_err[error_i] ^= 1;
        prev_errors.push_back(error_i);
    }
}

void Model::find_gen_polynom()
{
    gen_polynom = table3[1][h];
    int current_i = 3;
    while (current_i != i+2)
    {
        gen_polynom *= table3[current_i][h];
        current_i += 2;
    }
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
    while (ostatok.get_W() > size_t(s))
    {
        B_fixed <<= 1;
        ++shift_count;
        ostatok = B_fixed % gen_polynom;
    }
    B_fixed += ostatok;
    B_fixed >>= shift_count;
    if (B_fixed != B) B_fixed = 0;
}
