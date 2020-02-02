#include "trenirovka.h"
#include <random>
#include <ctime>
#include <algorithm>
#include <sstream>

// -- для работы рандома -- //
static time_t seed = time(nullptr);
static std::mt19937 mt(seed);

uint16_t Trenirovka::getPrimeNumber() const noexcept
{
    std::uniform_int_distribution<uint16_t> rand_range(0,7); // от 0 до 7, так как в массиве 8 элементов
    size_t i = rand_range(mt);
    return prime_array[i];
}

uint16_t Trenirovka::createNumber() noexcept
{
    std::uniform_int_distribution<uint16_t> predel_range(low_predel,up_predel);  // -- диапазон от 30 до 1000 -- //
    uint16_t predel = predel_range(mt); // -- сгенерированное число-задание не должно быть больше этого предела -- //
    uint16_t c = 1;
    bool end = false;   // -- если true, то прекратить генерацию числа -- //
    while (!end)
    {
        const uint16_t prime = getPrimeNumber();
        const uint16_t new_c = c * prime;
        if (new_c <= predel)
        {
            c = new_c;
            number_array.push_back(prime);
        }
        else
        {
            end = true; // -- так как число уже больше предела -- //
        }
    }
    return c;
}

std::wstring Trenirovka::getZadanie() noexcept
{
    uint16_t chislo = createNumber();
    std::wstring str = L"Разложите число " + std::to_wstring(chislo) + L" на простые множители.";
    return str;
}

std::wstring Trenirovka::getHint() noexcept
{
    // -- получаю случайный простой множитель из массива в качестве подсказки -- //
    std::uniform_int_distribution<size_t> rand_range(0,number_array.size()-1);
    size_t i = rand_range(mt);
    uint16_t chislo = number_array[i];
    std::wstring str = L"Подсказка: одним из множителей является число " + std::to_wstring(chislo) + L".";
    return str;
}

void Trenirovka::parseAnswer(const std::wstring &str) noexcept
{
    uint16_t chislo;
    std::wstringstream ss(str);
    while (ss >> chislo)
    {
        answer_array.push_back(chislo);
    }
}

bool Trenirovka::checkAnswer(const std::wstring &str) noexcept
{
    parseAnswer(str);   // -- заполняем массив - ответ пользователя -- //
    if (number_array.size() == answer_array.size())
    {
        std::sort(number_array.begin(),number_array.end()); // -- отсортируем массивы -- //
        std::sort(answer_array.begin(),answer_array.end());
        return (number_array == answer_array);
    }
    return false;
}

void Trenirovka::clear()
{
    number_array.clear();
    answer_array.clear();
}
