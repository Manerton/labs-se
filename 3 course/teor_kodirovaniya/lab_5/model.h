#ifndef MODEL_H
#define MODEL_H
#include <cstdint>
#include <vector>
#include <string>
#include <QDebug>
#include "bigbinary.h"

class Model
{
private:
    // альфа
    BigBinary A;
    BigBinary B_with_err;
    BigBinary B_fixed;
    // число информационных разрядов
    const int k;
    // число корректирующих разрядов
    int r = 0;
    int n = 0;
    // корректирующая способность кода
    const int t = 1;
    // кодовое расстояние
    const int d0;
    // количество сдвигов при поиске ошибки
    int shift_count = 0;
    // образующий многочлен
    BigBinary gen_polynom;
    // этапы процедуры кодирования
    BigBinary kodirovanie1;
    BigBinary kodirovanie2;
    BigBinary B;
    // номер разряда с ошибкой
    int error_razryad = 0;

    // методы
    void calculate_r(); // вычислить r и n
    void add_error_to_B();
    void find_gen_polynom();
    void kodirovanie();
    void find_error();
public:
    Model(const BigBinary &_a) : A(_a), k(_a.size()), d0(2*t+1) {
        calculate_r();  
        find_gen_polynom();
        kodirovanie();
        add_error_to_B();
        find_error();
    }

    int getR() const;
    int getN() const;
    BigBinary getB() const;
    BigBinary getB_with_err() const;
    int getError_razryad() const;
    BigBinary getGen_polynom() const;
    BigBinary getKodirovanie1() const;
    BigBinary getKodirovanie2() const;
    int getShift_count() const;
    BigBinary getB_fixed() const;
};

#endif // MODEL_H
