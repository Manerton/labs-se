#ifndef MODULO_H
#define MODULO_H


#include <iostream>
#include <string>

class Modulo
{
    uint64_t chislo;
    uint16_t N;
    void CheckForZeroModule(const uint16_t &N) const;
    void CheckForDifferentModules(const Modulo &b) const;

public:
    Modulo() : chislo(0), N(10) {}
    Modulo(const uint64_t chislo, uint16_t N)
    {
        CheckForZeroModule(N);
        this->chislo = chislo;
        this->N = N;
    }
    Modulo& operator+=(const Modulo &b);
    Modulo& operator-=(const Modulo &b);
    Modulo& operator*=(const Modulo &b);
    Modulo& operator/=(const Modulo &b);
    Modulo& operator++();
    Modulo operator++(int);
    std::string toString() const noexcept;

    friend Modulo operator+(const Modulo &a, const Modulo &b);
    friend Modulo operator-(const Modulo &a, const Modulo &b);
    friend Modulo operator*(const Modulo &a, const Modulo &b);
    friend Modulo operator/(const Modulo &a, const Modulo &b);

    //операции сравнения
    friend bool operator==(const Modulo &a, const Modulo &b);
    friend bool operator!=(const Modulo &a, const Modulo &b);
    friend bool operator<(const Modulo &a, const Modulo &b);
    friend bool operator>=(const Modulo &a, const Modulo &b);
    friend bool operator>(const Modulo &a, const Modulo &b);
    friend bool operator<=(const Modulo &a, const Modulo &b);
    friend std::ostream& operator<<(std::ostream& t, const Modulo &b); // вывод
    friend std::istream& operator>>(std::istream& t, Modulo &b); //ввод
    // исключения
    class zero_module_exception {};
    class different_modules_exception {};
};

#endif // MODULO_H
