#include "stats.h"
#include <sstream>
#include <fstream>
#include <ctime>

using namespace std;

void Stats::check_for_max_series() noexcept
{
    if (current_series_right > series_right) series_right = current_series_right;
    if (current_series_wrong > series_wrong) series_wrong = current_series_wrong;
}

void Stats::right() noexcept
{
    ++count_right;
    ++current_series_right;
    check_for_max_series();
    if (current_series_wrong > 0)
    {
        current_series_wrong = 0;
    }
    if (hintActivated)
    {
        ++count_hinted_right;
        hintActivated = false;
    }
}

void Stats::add_series_right()  // -- если серия правильных ответов была прервана (завершением работы тренажера или неправильным ответом), то записать эту серию -- //
{
    if (current_series_right > 0) // -- была ли вообще серия правильных ответов -- //
    {
        ++count_series_right;
        summa_series_right += current_series_right;
        current_series_right = 0;
    }
}

void Stats::wrong() noexcept
{
    ++count_wrong;
    ++current_series_wrong;
    check_for_max_series();
    add_series_right(); // -- серия правильных ответов была прервана, если она вообще была -- //
    if (hintActivated)
    {
        ++count_hinted_wrong;
        hintActivated = false;
    }
}

void Stats::statsToFile()
{
    wstringstream wss;
    calculate_percent_right();
    calculate_percent_wrong();
    calculate_avg_series_right();
    calculate_ratio();
    time_t td;
    td = time(nullptr);
    wss << L"Дата и время завершения тренировки: ";
    wss << ctime(&td);
    wss << L"Имя пользователя: " << username << endl;
    wss << L"Количество выполненных упражнений: " << count_complited << endl;
    wss << L"Количество использованных подсказок: " << count_hint << endl;
    wss << L"Количество повторений: " << count_repeat << endl;
    wss << L"Количество правильно выполненных упражнений: " << count_right << endl;
    wss << L"Количество правильно выполненных упражнений с подсказкой: " << count_hinted_right << endl;
    wss << L"Процент правильно выполненных упражнений: " << percent_right << L"%" << endl;
    wss << L"Количество неправильно выполненных упражнений: " << count_wrong << endl;
    wss << L"Количество неправильно выполненных упражнений с подсказкой: " << count_hinted_wrong << endl;
    wss << L"Процент неправильно выполненных упражнений: " << percent_wrong << L"%" << endl;
    wss << L"Серия правильных ответов максимальной длины: " << series_right << endl;
    wss << L"Серия неправильных ответов максимальной длины: " << series_wrong << endl;
    wss << L"Средний размер серии правильных ответов: " << avg_series_right << endl;
    if (ratio_right_wrong > 0 )
    {
        wss << L"Отношение количества правильных ответов к количеству неправильных: " << ratio_right_wrong << endl;
    }
    wss << L"---------------------------------------" << endl;

    wofstream file(filename, ios::app);
    if (file)
    {
        file << wss.str();
    }
}

bool Stats::checkName()
{
    // -- проверка, был ли изменено дефолтное имя (на случай, если программу закроют во время работы тренажера, тогда можно будет сохранить статистику) -- //
    // -- если же имя дефолтное, значит тренажер не начинал/закончил свою работу и сохранять статистику не надо -- //
    return (username == L"noname");
}

void Stats::changeName(const wstring &ws)
{

    if (!ws.empty()) username = ws;
    else username = L"Аноним";
}

wstring Stats::statsFromFile()
{
    std::wifstream file(filename);
    if (file)
    {
        std::wostringstream wss;
        wstring ws;
        while (getline(file,ws))
        {
            wss << ws << L"\r\n";
        }
        return {wss.str()};
    }
    else
    {
        return wstring(L"Файла со статистикой не существует.");
    }
}
