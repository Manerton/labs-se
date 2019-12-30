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
    Triad(int64_t _num1 = 0, int64_t _num2 = 0, int64_t _num3 = 0) noexcept
        : num1(_num1), num2(_num2), num3(_num3) {}

    std::string toString() const noexcept; // -- перевод Triad в строку -- //

    // -- методы получения значений полей -- //
    inline int64_t getNum1() const noexcept { return num1; }
    inline int64_t getNum2() const noexcept { return num2; }
    inline int64_t getNum3() const noexcept { return num3; }

    // -- методы установки значений для полей -- //
    inline void setNum1(int64_t b) noexcept { num1 = b; }
    inline void setNum2(int64_t b) noexcept { num2 = b; }
    inline void setNum3(int64_t b) noexcept { num3 = b; }

    // -- методы сложения отдельно для каждого числа из тройки -- //
    inline void add_to_num1(int64_t b) noexcept { num1 += b; }
    inline void add_to_num2(int64_t b) noexcept { num2 += b; }
    inline void add_to_num3(int64_t b) noexcept { num3 += b; }

    // -- методы умножения отдельно для каждого числа из тройки -- //
    inline void mul_num1(int64_t b) noexcept { num1 *= b; }
    inline void mul_num2(int64_t b) noexcept { num2 *= b; }
    inline void mul_num3(int64_t b) noexcept { num3 *= b; }

    // -- операции-методы присваивания -- //
    Triad& operator+=(int64_t b);  // -- прибавления числа к каждому числу тройки -- //
    Triad& operator*=(int64_t b); // -- умножение каждого числа тройки на число b -- //
    // -- дружеские операции -- //
    friend Triad operator+(const Triad &a, int64_t b);
    friend Triad operator*(const Triad &a, int64_t b);
    // -- операции сравнения и вывода -- //
    friend bool operator==(const Triad &a, const Triad &b);
    friend bool operator!=(const Triad &a, const Triad &b);
    friend std::ostream& operator<<(std::ostream& t, const Triad &r);
    friend std::istream& operator>>(std::istream& t, Triad &r);

};

#endif // TRIAD_H
