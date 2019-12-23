#ifndef MONEY_H
#define MONEY_H
#include <cinttypes>
#include <string>
class Money // -- класс для работы с денежными суммами -- //
{
    uint64_t rubles; // -- рубли -- //
    uint8_t kopecks; // -- копейки -- //
    void CheckConversionFactor(const double& factor); // -- коэффициент должен быть > 0 -- //
public:
    Money(uint64_t _rubles = 0, uint8_t _kopecks = 0) : rubles(_rubles), kopecks(_kopecks)
    {
        while (kopecks >= 100) {kopecks -= 100; ++rubles;}  // -- на случай, если копеек >= 100 - //
    }
    std::string toString() const noexcept; // -- перевод денег в строку для вывода -- //
    // -- операции-методы присваивания -- //
    Money& operator+=(const Money &b);  // -- сложение сумм -- //
    Money& operator-=(const Money &b);  // -- вычитание сумм -- //
    Money& operator/=(const Money &b);  // -- деление сумм -- //
    Money& operator/=(const double &b); // -- деление суммы на дробное число -- //
    Money& operator*=(const double &b); // -- умножение на дробное число -- //
    // -- дружеские операции -- //
    friend Money operator+(const Money &a, const Money &b);
    friend Money operator-(const Money &a, const Money &b);
    friend Money operator/(const Money &a, const Money &b);
    friend Money operator+(const Money &a, const double &b);
    // -- операции сравнения и вывода -- //
    friend bool operator==(const Money &a, const Money &b);
    friend bool operator!=(const Money &a, const Money &b);
    friend bool operator<(const Money &a, const Money &b);
    friend bool operator>=(const Money &a, const Money &b);
    friend bool operator>(const Money &a, const Money &b);
    friend bool operator<=(const Money &a, const Money &b);
    friend std::ostream& operator<<(std::ostream& t, const Money &r);
    friend std::istream& operator<<(std::istream& t, Money &r);
    // -- исключения -- //
    class divide_by_zero_exception {};    // -- деление на ноль -- //
    class negative_conversion_factor_exception {} ;   // -- коэффициент пересчета отрицательный (дробное число, используемое при делении) -- //
};


#endif // MONEY_H
