#ifndef MODEL_H
#define MODEL_H
#include <cstdint>
#include <vector>
#include <string>
#include <QDebug>
#include "bigbinary.h"

class Model
{
public:
    struct table2_row
    {
        int n;
        int k;
        int r;
        int s;
    };
    using Table2 = std::vector<table2_row>;
    using table3_row = std::vector<BigBinary>;
    using Table3 = std::map<int,table3_row>;
private:
    // соотношение корректирующих и информационных разрядов для БЧХ кодов
    static Table2 table2;
    // Минимальные неприводимые многочлены в поле Галуа GF(2)
    static Table3 table3;
    // альфа
    BigBinary A;
    BigBinary B_with_err;
    BigBinary B_fixed;
    // число информационных разрядов
    const int k;
    // число корректирующих разрядов
    int r = 0;
    int n = 0;
    int s = 0;
    // для поиска порождающего многочлена
    int h = 0;
    int i = 0;
    // количество сдвигов при поиске ошибки
    int shift_count = 0;
    // образующий многочлен
    BigBinary gen_polynom;
    // этапы процедуры кодирования
    BigBinary kodirovanie1;
    BigBinary kodirovanie2;
    BigBinary B;
    // количество ошибок
    int m = 0;

    // методы
    void calculate_r_n_s(); // вычислить r и n
    void calculate_h_i();   // вычислить h и i
    void add_error_to_B();
    void find_gen_polynom();
    void kodirovanie();
    void find_error();
public:
    Model(const BigBinary &_a) : A(_a), k(_a.size()) {
        calculate_r_n_s();
        calculate_h_i();
        find_gen_polynom();
        kodirovanie();
        add_error_to_B();
        find_error();
    }
    int getR() const;
    int getN() const;
    int getS() const;
    int getH() const;
    int getI() const;
    int getM() const;
    BigBinary getB() const;
    BigBinary getB_with_err() const;
    BigBinary getGen_polynom() const;
    BigBinary getKodirovanie1() const;
    BigBinary getKodirovanie2() const;
    int getShift_count() const;
    BigBinary getB_fixed() const;
    static void setTable2(const std::vector<table2_row> &value);
    static void setTable3(const Table3 &value);
};

#endif // MODEL_H
