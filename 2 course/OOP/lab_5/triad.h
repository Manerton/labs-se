#ifndef TRIAD_H
#define TRIAD_H
#include <cinttypes>
#include <string>

/* 11.	Создать базовый класс Triad (тройка чисел) с операциями сложения с числом, умножения на число, проверки на равенство.
 * Создать производный класс vector3D, задаваемый тройкой координат.
 * Должны быть реализованы: операция сложения векторов, скалярное произведение векторов. */

class Triad
{
    int64_t num1;
    int64_t num2;
    int64_t num3;
public:
    Triad(const int64_t& _num1, const int64_t& _num2, const int64_t& _num3)
        : num1(_num1), num2(_num2), num3(_num3){}

    std::string toString() const noexcept; // -- перевод денег Triad в число -- //
    // -- методы получения значений полей -- //
    int64_t getNum1() const noexcept;
    int64_t getNum2() const noexcept;
    int64_t getNum3() const noexcept;

    // -- методы установки значений для полей -- //
    void setNum1(const int64_t &b) noexcept;
    void setNum2(const int64_t &b) noexcept;
    void setNum3(const int64_t &b) noexcept;

    // -- методы сложения отдельно для каждого числа из тройки -- //
    void add_to_num1(const int64_t &b) noexcept;
    void add_to_num2(const int64_t &b) noexcept;
    void add_to_num3(const int64_t &b) noexcept;

    // -- методы умножения отдельно для каждого числа из тройки -- //
    void mul_num1(const int64_t &b) noexcept;
    void mul_num2(const int64_t &b) noexcept;
    void mul_num3(const int64_t &b) noexcept;

    // -- операции-методы присваивания -- //
    Triad& operator+=(const int64_t &b);  // -- прибавления числа к каждому числу тройки -- //
    Triad& operator*=(const int64_t &b); // -- умножение каждого числа тройки на число b -- //
    // -- дружеские операции -- //
    friend Triad operator+(const Triad &a, const int64_t &b);
    friend Triad operator*(const Triad &a, const int64_t &b);
    // -- операции сравнения и вывода -- //
    friend bool operator==(const Triad &a, const Triad &b);
    friend bool operator!=(const Triad &a, const Triad &b);
    friend std::ostream& operator<<(std::ostream& t, const Triad &r);
    friend std::istream& operator>>(std::istream& t, Triad &r);

};

#endif // TRIAD_H
