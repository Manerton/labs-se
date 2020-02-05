#ifndef STATS_H
#define STATS_H

#include <cinttypes>
#include <string>
#include <iostream>

class Stats // -- класс для сбора статистики -- //
{
    std::string filename = "stats.txt";

    std::wstring username = L"noname";

    bool hintActivated = false;

    uint64_t count_complited = 0; // -- количество выполненных упражнений -- //
    uint64_t count_hint = 0; // -- количество использованных подсказок -- //
    uint64_t count_repeat = 0; // -- количество повторений заданий (нажатие на кнопку Повторить) -- //
    uint64_t count_right = 0; // -- количество правильно выполненных упражнений -- //
    uint64_t count_hinted_right = 0; // -- количество правильно выполненных упражнений  с подсказкой -- //
    double percent_right = 0; // -- процент правильно выполненных упражнений -- //
    uint64_t count_wrong = 0; // -- количество неправильно выполненных упражнений -- //
    uint64_t count_hinted_wrong = 0; // -- количество неправильно выполненных упражнений с подсказкой -- //
    double percent_wrong = 0; // -- процент неправильно выполненных упражнений -- //
    uint64_t current_series_right = 0;  // -- текущая серия правильных ответов -- //
    uint64_t series_right = 0; // -- серия правильных ответов максимальной длины -- //
    uint64_t current_series_wrong = 0;  // -- текущая серия неправильных ответов -- //
    uint64_t series_wrong = 0; // -- серия правильных ответов максимальной длины -- //
    uint64_t count_series_right = 0; // -- количество серий правильных ответов -- //
    double avg_series_right = 0; // -- средний размер серии правильных ответов -- //
    uint64_t summa_series_right = 0; // -- сумма всех серий правильных ответов -- //
    double ratio_right_wrong = 0; // -- отношение количества правильных ответов к количеству неправильных -- //

    // -- методы -- //
    void check_for_max_series() noexcept;   // -- если текущая серия больше максимальной, то теперь максимальная = текущая -- //
    void add_series_right(); // -- если серия правильных ответов была прервана (завершением работы тренажера или неправильным ответом), то записать эту серию -- //
    void calculate_percent_right() noexcept
    {
        if (count_complited > 0)
        {
            percent_right = 100 * double(count_right)/count_complited;
        }

    }
    void calculate_percent_wrong() noexcept
    {
        if (count_complited > 0)
        {
            percent_wrong = 100 * double(count_wrong)/count_complited;
        }
    }
    void calculate_avg_series_right() noexcept
    {
        add_series_right();
        if (count_series_right > 0)
        {
            avg_series_right = double(summa_series_right) / count_series_right;
        }
    }
    void calculate_ratio() noexcept
    {
        if (count_wrong > 0)
        {
            ratio_right_wrong = double(count_right)/count_wrong;
        }
    }
public:
    Stats() = default;
    void complete() noexcept {++count_complited;}
    void hint() noexcept
    {
        ++count_hint;
        hintActivated = true;
    }
    void repeat() noexcept {++count_repeat;}
    void right() noexcept;
    void wrong() noexcept;
    void getStats();
    void statsToFile();
    bool checkName();
    void changeName(const std::wstring &ws);
    std::wstring statsFromFile();
};

#endif // STATS_H
