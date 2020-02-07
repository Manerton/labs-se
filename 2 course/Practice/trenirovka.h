#ifndef TRENIROVKA_H
#define TRENIROVKA_H
#include <cinttypes>
#include <vector>
#include <array>
#include <string>

class Trenirovka    // -- класс для тренажера -- //
{
    const std::array<uint8_t,8> prime_array = {2,3,5,7,11,13,17,19};    // -- массив простых чисел от 2 до 19 -- //
    const uint16_t low_predel = 30; // -- нижний предел -- //
    const uint16_t up_predel = 1000;    // -- верхний предел -- //
    uint16_t chislo; // -- число-задание -- //
    std::vector<uint16_t> number_array; // -- массив, состоящий из простых множителей числа-задания -- //
    std::vector<uint16_t> answer_array; // -- массив-ответ пользователя -- //
    // -- для предыдущего задания (для повтора) -- //
    std::vector<uint16_t> previous_number_array; // -- массив, состоящий из простых множителей числа-задания для предыдущего задания -- //
    uint16_t prevChislo;   // -- предыдущее число-задание -- //
    // -- методы -- //
    uint16_t getPrimeNumber() const noexcept;   // -- получить случайное простое число из массива prime_array -- //
    uint16_t createNumber() noexcept;    // -- сформировать массив number_array и получить число-задание для getZadanie -- //
    void parseAnswer(const std::wstring &str) noexcept; // -- разобрать ответ и заполнить массив answer_array -- //
    bool Repeated = false;
public:
    Trenirovka() = default;
    std::wstring getZadanie() noexcept;    // -- получить строку с заданием (с числом, которое надо разложить) -- //
    std::wstring getPrevZadanie() noexcept; // -- получить предыдущее задание -- //
    void savePrevZadanie() noexcept;    // -- сохранить предыдущее задание -- //
    std::wstring getHint() noexcept;       // -- получить подсказку -- //
    bool checkAnswer(const std::wstring &str) noexcept; // -- проверить ответ -- //
    void clear();
};

#endif // TRENIROVKA_H
