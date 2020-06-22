const int MAX_COUNT = 1000000;

#include <iostream>
#include <random>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <limits>

#include <windows.h>

using namespace std;

// генерируем случайные вещественные числа согласно параметрам
vector<double> generate_chisla(const size_t& N, double& min, double& max)
{
    vector<double> res(N);
    if (min > max) swap(min, max);
    time_t seed = time(nullptr);
    mt19937 mt(seed);
    uniform_real_distribution<double> dist(min, max);    // значения от мин до макс
    // генерируем числа в заданном диапазоне
    for (size_t i = 0; i < N; ++i) res[i] = dist(mt);

    return res;
}

// заполнение случайными числами
void fill_random_file(LPCWSTR FileName, const vector<double>& chisla)
{
    // создаю файл (открываю на запись)
    HANDLE hFile = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        cout << "Не удалось создать файл";
    }
    else
    {
        DWORD dwBytes;
        size_t N = chisla.size();
        for (size_t i = 0; i < N; ++i)
        {
            WriteFile(hFile, &chisla[i], sizeof(double), &dwBytes, nullptr);
        }
        CloseHandle(hFile); // закрываю файл
    }
}

// вывод данных из файла на экран
void out_from_file(LPCWSTR FileName)
{
    // открываю на чтение файл
    HANDLE hFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        cout << "Не удалось открыть файл";
    }
    else
    {
        DWORD dwBytes;
        double tmp;
        DWORD double_count = GetFileSize(hFile, nullptr) / sizeof(double);
        for (DWORD i = 0; i < double_count; ++i)
        {
            ReadFile(hFile, &tmp, sizeof(double), &dwBytes, nullptr);
            cout << tmp << ' ';
        }
        cout << endl;
        CloseHandle(hFile);
    }
}

// переписывание из bin в txt файл
void from_bin_to_txt(LPCWSTR BinFileName, LPCWSTR TxtFileName)
{
    HANDLE hFileB = CreateFile(BinFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); // открытие на чтение
    HANDLE hFileT = CreateFile(TxtFileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); // открытие на Запись
    if (INVALID_HANDLE_VALUE == hFileB || INVALID_HANDLE_VALUE == hFileT)
    {
        cout << "Не удалось открыть файл";
    }
    else
    {
        double tmp;
        DWORD double_count = GetFileSize(hFileB, nullptr) / sizeof(double);
        DWORD dwBytes;
        // чтение из бинарного файла и запись в текстовый
        for (DWORD i = 0; i < double_count; ++i)
        {
            // читаем вещественные числа
            ReadFile(hFileB, &tmp, sizeof(double), &dwBytes, nullptr);
            // превращаем вещ. число в строку
            stringstream ss;
            ss << tmp << ' ';
            for(char a : ss.str())
            {
                WriteFile(hFileT, &a, sizeof(char), &dwBytes, nullptr);
            }
        }
        CloseHandle(hFileB);
        CloseHandle(hFileT);
    }
}

// поиск минимального числа в файле
double find_min(HANDLE hFile)
{
    SetFilePointer(hFile, 0, 0, FILE_BEGIN);
    double result, tmp;
    DWORD dwBytes;
    DWORD double_count = GetFileSize(hFile, nullptr) / sizeof(double); // количество вещественных чисел в файле
    ReadFile(hFile, &result, sizeof(double), &dwBytes, nullptr);          // прочитаем первое число в result (возьмем его за минимум)
    for (DWORD i = 1; i < double_count; ++i)
    {
        ReadFile(hFile, &tmp, sizeof(double), &dwBytes, nullptr);         // прочитаем число в tmp
        if (tmp < result) result = tmp;
    }
    return result;
}

// поиск максимального числа в файле
double find_max(HANDLE hFile)
{
    SetFilePointer(hFile, 0, 0, FILE_BEGIN);
    double result, tmp;
    DWORD dwBytes;
    DWORD double_count = GetFileSize(hFile, nullptr) / sizeof(double); // количество вещественных чисел в файле
    ReadFile(hFile, &result, sizeof(double), &dwBytes, nullptr);          // прочитаем первое число в result (возьмем его за максимум)
    for (DWORD i = 1; i < double_count; ++i)
    {
        ReadFile(hFile, &tmp, sizeof(double), &dwBytes, nullptr);         // прочитаем число в tmp
        if (tmp > result) result = tmp;
    }
    return result;
}

// поиск среднеарифметического в файле
double find_sred_arifm(HANDLE hFile)
{
    SetFilePointer(hFile, 0, 0, FILE_BEGIN);
    double result=0, tmp;
    DWORD dwBytes;
    DWORD double_count = GetFileSize(hFile, nullptr) / sizeof(double); // количество вещественных чисел в файле
    for (DWORD i = 0; i < double_count; ++i)
    {
        ReadFile(hFile, &tmp, sizeof(double), &dwBytes, nullptr);       // прочитаем число в tmp
        result += tmp;
    }

    return double(result/double_count);
}

// запись выходного бинарного файла в соответствии с заданием варианта 7
void write_file_for_variant(LPCWSTR FromFileName, LPCWSTR ToFileName)
{
    HANDLE hFile = CreateFile(FromFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hFileWrite = CreateFile(ToFileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile || INVALID_HANDLE_VALUE == hFileWrite)
    {
        cout << "Не удалось открыть файл";
    }
    else
    {
        // ищем все необходимое для выполнения задания варианта 7
        double min = find_min(hFile);
        double max = find_max(hFile);
        double fabs_diff = fabs(min-max);
        double sred_arifm = find_sred_arifm(hFile);
        DWORD dwBytes;
        DWORD double_count = GetFileSize(hFile, nullptr) / sizeof(double); // количество чисел в файле
        SetFilePointer(hFile, 0, 0, FILE_BEGIN); // устанавливаем позицию в начало файла

        double chislo;
        for (DWORD i = 0; i < double_count; ++i)
        {
            ReadFile(hFile, &chislo, sizeof(double), &dwBytes, nullptr);
            if (chislo < sred_arifm) chislo /= fabs_diff;
            WriteFile(hFileWrite, &chislo, sizeof(double), &dwBytes, nullptr);
        }
        CloseHandle(hFile);
        CloseHandle(hFileWrite);
    }
}

// дописываем данные из одного файла в конец другого
void add_to_file(LPCWSTR FromFileName, LPCWSTR ToFileName)
{
    // из какого файла берем данные для дозаписи
    HANDLE hFile = CreateFile(FromFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    // в какой файл дописываем
    HANDLE hFileWrite = CreateFile(ToFileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile || INVALID_HANDLE_VALUE == hFileWrite)
    {
        cout << "Не удалось открыть файл";
    }
    else
    {
        DWORD dwBytes;
        DWORD double_count = GetFileSize(hFile, nullptr) / sizeof(double); // количество чисел в файле
        SetFilePointer(hFileWrite, 0, 0, FILE_END); // установка позиции в начало файла

        double chislo;
        for (DWORD i = 0; i < double_count; ++i)
        {
            ReadFile(hFile, &chislo, sizeof(double), &dwBytes, nullptr);
            WriteFile(hFileWrite, &chislo, sizeof(double), &dwBytes, nullptr);
        }
        CloseHandle(hFile);
        CloseHandle(hFileWrite);
    }
}

// перезапись файла полученного в результате задания 5 с выполнением задания варианта
void rewrite_file_for_variant(LPCWSTR FileName)
{
    HANDLE hFile = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); // открытие на запись
    if (INVALID_HANDLE_VALUE == hFile)
    {
        cout << "Не удалось открыть файл";
    }
    else
    {
        double min = find_min(hFile);
        double max = find_max(hFile);
        double fabs_diff = fabs(min-max);
        double sred_arifm = find_sred_arifm(hFile);

        DWORD dwBytes;
        DWORD Bytes_count = GetFileSize(hFile, nullptr); // количество байтов в файле
        SetFilePointer(hFile, 0, 0, FILE_BEGIN); // установка позиции в начало файла

        double chislo;
        DWORD offset = 0; // для хранения позиции чтения и записи
        // читаем числа пока не дошли до конца файла
        while(offset < Bytes_count)
        {
            SetFilePointer(hFile, offset, 0, FILE_BEGIN); // установим позицию чтения
            ReadFile(hFile, &chislo, sizeof(double), &dwBytes, nullptr); // читаем число
            if (chislo < sred_arifm) // если число меньше среднего арифметического
            {
                chislo /= fabs_diff; // делим на абсолютную величину разности мин. и макс. числа файла
                SetFilePointer(hFile, offset, 0, FILE_BEGIN); // возвращаемся на это число чтобы его заменить
                WriteFile (hFile, &chislo, sizeof(double), &dwBytes, nullptr);
            }
            offset += sizeof(double); // для перехода к следующему числу
        }
        CloseHandle(hFile);
    }
}

// для заполнения текстового файла бинарным в соответствии с заданием 7 варианта
void rewrite_file_for_variant(LPCWSTR BinFileName, LPCWSTR TxtFileName)
{
    // из какого файла будет чтение
    HANDLE hFile = CreateFile(BinFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    // куда будет записано
    HANDLE hFileWrite = CreateFile(TxtFileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile || INVALID_HANDLE_VALUE == hFileWrite)
    {
        cout << "Не удалось открыть файл";
    }
    else
    {
        double min = find_min(hFile);
        double max = find_max(hFile);
        double fabs_diff = fabs(min-max);
        double sred_arifm = find_sred_arifm(hFile);
        DWORD dwBytes;
        DWORD double_count = GetFileSize(hFile, nullptr) / sizeof(double); // количество чисел в файле
        SetFilePointer(hFile, 0, 0, FILE_BEGIN); // установка позиции в начало файла
        double chislo;
        // читаем числа пока не дошли до конца файла
        for (DWORD i = 0; i < double_count; ++i)
        {
            ReadFile(hFile, &chislo, sizeof(double), &dwBytes, 0); // читаем число
            if (chislo < sred_arifm) chislo /= fabs_diff;// если число меньше сред ариф
            stringstream ss;
            ss << chislo << ' ';
            for(char a : ss.str())
            {
                WriteFile(hFileWrite, &a, sizeof(char), &dwBytes, nullptr);
            }
        }
        CloseHandle(hFile);
        CloseHandle(hFileWrite);
    }
}

// перезапись по условиям варианта из текстового в бинарный
void rewrite_file_for_variant_T_to_B(LPCWSTR TxtFileName, LPCWSTR BinFileName)
{
    // из какого файла будет чтение
    HANDLE hFile = CreateFile(TxtFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    // куда будет записано
    HANDLE hFileWrite = CreateFile(BinFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile || INVALID_HANDLE_VALUE == hFileWrite)
    {
        cout << "Не удалось открыть файл";
    }
    else
    {
        double min = find_min(hFile);
        double max = find_max(hFile);
        double fabs_diff = fabs(min-max);
        double sred_arifm = find_sred_arifm(hFile);

        DWORD dwBytes;
        DWORD Bytes_count = GetFileSize(hFile, nullptr); // размер файла

        stringstream ss;
        char num;
        SetFilePointer(hFile, 0, 0, FILE_BEGIN); // установка позиции в начало файла

        for (DWORD i = 0; i < Bytes_count; ++i)
        {
            ReadFile(hFile, &num, sizeof(char), &dwBytes, nullptr); // читаем число посимвольно в строковый поток
            ss << num;
        }
        DWORD double_count = Bytes_count / sizeof(double);
        for (DWORD i = 0; i < double_count; ++i)
        {
            double chislo;
            ss >> chislo;
            if (chislo < sred_arifm) chislo /= fabs_diff; // если число отрицательное
            WriteFile(hFileWrite, &chislo, sizeof(double), &dwBytes, nullptr);
        }

        CloseHandle(hFile);
        CloseHandle(hFileWrite);
    }
}

bool compare(LPCWSTR Bin1_FileName, LPCWSTR Bin2_FileName)
{
    HANDLE hFile1 = CreateFile(Bin1_FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hFile2 = CreateFile(Bin2_FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile1 || INVALID_HANDLE_VALUE == hFile2)
    {
        cout << "Не удалось открыть файл";
    }
    else
    {
        if (GetFileSize(hFile1, nullptr) != GetFileSize(hFile2, nullptr)) return false; // разное количество байтов, значит не равны
        DWORD dwBytes;
        DWORD double_count = GetFileSize(hFile1, nullptr) / sizeof(double);
        double a, b;
        for (DWORD i = 0; i < double_count; ++i) // считываем все числа из файлов и сравниваем друг с другом
        {
            ReadFile(hFile1, &a, sizeof(double), &dwBytes, nullptr); // читаем число из файла Bin1
            ReadFile(hFile2, &b, sizeof(double), &dwBytes, nullptr); // читаем число из файла Bin2
            if (fabs(a - b) > numeric_limits<double>::epsilon()) return false; // сравниваем, если разность больше epsilon значит не равны
        }
        return true; // числа равны
    }
    return false;
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №12. Вариант 7.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    cout << "Записать выходной файл, поделив все числа, меньшие среднего арифметического на абсолютную величину разности минимального и максимального чисел." << endl;
    size_t N;
    double min, max;
    cout << "Введите N, min, max:" << endl;
    cin >> N >> min >> max;
    // проверяем
    if (N > MAX_COUNT) return 1; // превышено количество
    if (fabs(max - min) < numeric_limits<double>::epsilon()) return 1; // они равны
    // выполняем задания
    cout << "Задание 1. Создать бинарный файл, который будет считаться исходным (input.bin)" << endl;
    fill_random_file(L"input.bin", generate_chisla(N, min, max));
    system("pause");

    cout << "Задание 2. Вывести содержимое исходного бинарного файла на экран и в текстовый файл (text1.txt)" << endl;
    out_from_file(L"input.bin");
    from_bin_to_txt(L"input.bin", L"text1.txt");
    system("notepad.exe text1.txt");
    system("pause");

    cout << "Задание 3. Записать выходной бинарный файл в соответствии с заданием своего варианта (output.bin)" << endl;
    write_file_for_variant(L"input.bin", L"output.bin");
    system("pause");

    cout << "Задание 4. Вывести содержимое выходного бинарного файла на экран и в файл (text2.txt)" << endl;
    out_from_file(L"output.bin");
    from_bin_to_txt(L"output.bin", L"text2.txt");
    system("notepad.exe text2.txt");
    system("pause");

    cout << "Задание 5. Дописать к содержимому бинарного файла input.bin содержимое бинарного файла output.bin" << endl;
    add_to_file(L"output.bin", L"input.bin");
    system("pause");

    cout << "Задание 6. Вывести содержимое полученного бинарного файла на экран и в файл (text3.txt)" << endl;
    out_from_file(L"input.bin");
    from_bin_to_txt(L"input.bin", L"text3.txt");
    system("notepad.exe text3.txt");
    system("pause");

    cout << "Задание 7. Выполнить перезапись по месту бинарного файла, полученного в задании №5 (input.bin)" << endl
         << "в соответствии с заданием своего варианта (перезапись по месту означает, что из файла читается" << endl
         << "очередное число; если оно не подлежит замене, то процесс чтения продолжается; если число подлежит" << endl
         << "замене, то необходимо записать на его место требуемое число; дополнительные файлы не использовать)." << endl;

    rewrite_file_for_variant(L"input.bin");
    system("pause");

    cout << "Задание 8. Вывести содержимое полученного бинарного файла (input.bin) на экран и в выходной" << endl
         << "текстовый файл (text4.txt) в соответствии с заданием своего варианта. Выходное число преобразуется" << endl
         << "в строку с использованием строкового потока" << endl;
    out_from_file(L"input.bin");
    rewrite_file_for_variant(L"input.bin", L"text4.txt");
    system("notepad.exe text4.txt");
    system("pause");

    cout << "Задание 9. Содержимое текстового файла (text4.txt), полученного в результате выполнения задания №8\n"
                 "переписать в соответствии с условиями своего варианта в бинарный файл (out2.bin). Входное число \n"
                 "считывается как строка и преобразуется во внутренний формат с помощью входного строкового потока. " << endl;
    rewrite_file_for_variant_T_to_B(L"text4.txt", L"out2.bin");
    system("pause");

    cout << "Задание 10. Вывести содержимое полученного бинарного файла (out2.bin) на экран и в файл" << endl;
    out_from_file(L"out2.bin");
    from_bin_to_txt(L"out2.bin", L"text5.txt");
    system("notepad.exe text5.txt");
    system("pause");

    cout << "Задание 11. Написать функцию для сравнения файлов и проверить, совпал ли файл, полученный в результате \n"
                 "выполнения задания №9(out2.bin) с файлом input.bin." << endl;
    if (compare(L"out2.bin", L"input.bin")) cout << "out2.bin == input.bin" << endl;
    else cout << "out2.bin != input.bin" << endl;
    system("pause");
    return 0;
}
