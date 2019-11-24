#ifndef MODULO_H
#define MODULO_H
#include <iostream>
#include <string>

class Modulo
{
    static const uint16_t NO_MODULO = UINT16_MAX; // для чисел без модуля, UINT16_MAX так как поле N - 16 битное, и это макс. модуль
    uint64_t chislo;
    uint16_t N;
    void CheckForZeroModule(const uint16_t &N) const;
    bool CheckForNoModulo() const;
    void CheckForDifferentModules(const Modulo &b) const;
    void CheckForExistInverseElem(const uint16_t &i) const;
    uint16_t GetInverseElement(const uint64_t &b) const; // получаю обратный элемент, который необходим для операции деления


public:
    Modulo() noexcept : chislo(0), N(10) {}
    Modulo(const uint64_t &in_chislo, const uint16_t &in_N)
    {
        CheckForZeroModule(in_N);
        chislo = in_chislo;
        N = in_N;
        if (chislo > N) chislo %= N;
    }

    Modulo(const double &in_chislo) // конструктор преобразования
    {
        chislo = static_cast<uint64_t>(in_chislo);
        N = NO_MODULO;
        if (chislo > N) chislo %= N;
    }

    std::string toString() const noexcept;
    // операции присваивания
    Modulo& operator+=(const Modulo &b);
    Modulo& operator-=(const Modulo &b);
    Modulo& operator*=(const Modulo &b);
    Modulo& operator/=(const Modulo &b);

    Modulo& operator++() noexcept;
    Modulo operator++(int) noexcept;

    Modulo& operator--() noexcept;
    Modulo operator--(int) noexcept;

    // арифметические операции
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
    class zero_module_exception {}; // нулевой модуль
    class different_modules_exception {}; // разные модули у чисел
    class inverse_elem_not_exist_exception {}; // не существует обратного элемента, т.е деление невозможно
};

#endif // MODULO_H
