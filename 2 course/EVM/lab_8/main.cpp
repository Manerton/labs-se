#include <iostream>
#include <cassert>
#include <cmath>
#include <random>
#include <iomanip>

using namespace std;

const size_t SIZE = 50;

void fill_array(double* arr, size_t N, double min, double max)
{
    if (min > max) swap(min, max);
    time_t seed = time(nullptr);
    mt19937 mt(seed);
    uniform_real_distribution<double> dist(min, max);    // значения от мин до макс
    // генерируем числа в заданном диапазоне
    for (size_t i = 0; i < N; ++i) arr[i] = dist(mt);
}

void display_array(double* arr, size_t N)
{
    for (size_t i = 0; i < N; ++i)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

//Найти среднее арифметическое положительных элементов данной последовательности
double sred_arif_cpp(double* arr, size_t N)
{
    double result = 0;
    size_t count = 0;
    for (size_t i = 0; i < N; ++i)
    {
        if (arr[i] > 0)
        {
            result += arr[i];
            ++count;
        }
    }
    if (count == 0) return result;
    else return double(result/count);
}

//Найти среднее арифметическое положительных элементов данной последовательности на ASM
double sred_arif_asm(double* arr, size_t N)
{
    double result = 0;
    size_t count = 0; // количество положительных элементов

    asm __volatile__ (
            R"(
                mov rdi, 0      # это индекс массива
                mov rcx, %[N]   # это количество итераций для цикла
                fld %[result]   # загружаем наш результат
             cycle:
                fld %[arr][(rdi*8)]   # загрузили элемент массива a в стек
                fldz                  # загрузили 0 в стек
                fcomip st, st(1)      # выясняем, является ли элемент положительным, вытесняем 0 из стека
                ja negative_num       # если элемент отрицательный, то переходим к negative_num
                # иначе
                inc %[count]          # увеличиваем счетчик положительных элементов
                fadd st(1), st

                negative_num:
                inc rdi               # увеличиваем индекс
                fstp st

                loop cycle

                fstp %[result]

            )"
            : [result] "+m" (result), [count] "+m" (count)
            : [arr] "m" (*arr), [N] "m" (N)
        );
    if (count == 0) return result;
    else return double(result/count);
}

// Найти номер максимального элемента в этой последовательности
size_t find_index_of_max_elem_cpp(double* arr, size_t N)
{
    double temp = arr[0];
    size_t index = 0;
    for (size_t i = 1; i < N; ++i)
    {
        if (arr[i] > temp)
        {
            temp = arr[i];
            index = i;
        }
    }
    return index;
}

// Найти номер максимального элемента в этой последовательности на ASM
double find_index_of_max_elem_asm(double* arr, size_t N)
{
    size_t index = 0;
    double test = 0;
    asm __volatile__ (
            R"(
                mov rdi, 1      # это индекс массива
                mov rcx, %[N]   # это количество итераций для цикла
                fld %[arr][0]   # загружаем первый элемент и берем его за максимум
            cycle2:
                fld %[arr][(rdi*8)]   # загрузили элемент массива в стек
                fcomi st, st(1)       # выясняем, больше ли этот элемент чем наш максимум
                jbe not_new_max       # если нет, то переходим к not_new_max
                # иначе
                fst st(1)             # новый максимум
                mov %[index], rdi     # новый индекс максимума

                not_new_max:

                inc rdi               # увеличиваем индекс массива
                fstp st               # выгружаем из стека элемент массива, который только что загружали

                loop cycle2
            )"
            : [index] "+m" (index), [test] "+m" (test)
            : [arr] "m" (*arr), [N] "m" (N)
        );
    return index;
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №8. Вариант 7.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    cout << "Задание 1: С клавиатуры вводят 20 действительных чисел." << endl
         << "Найти среднее арифметическое положительных элементов данной последовательности." << endl;
    const double eps = 0.00001; // -- точность сравнения при ассерте -- //
    double *arr = new double[SIZE];
    fill_array(arr,SIZE,-100,100);
    cout << "Исходный массив: " << endl;
    display_array(arr,SIZE);
    double res1_cpp = sred_arif_cpp(arr, SIZE);
    double res1_asm = sred_arif_asm(arr, SIZE);
    cout << res1_cpp << endl;
    cout << res1_asm << endl;
    assert(fabs(res1_cpp - res1_asm) <= eps);
    cout << "Задание 2: Дана последовательность из n целых чисел." << endl
         << "Найти номер максимального элемента в этой последовательности." << endl;
    size_t res2_cpp = find_index_of_max_elem_cpp(arr,SIZE)+1; // нужен номер, а не индекс
    double res2_asm = find_index_of_max_elem_asm(arr,SIZE-1)+1;
    cout << res2_cpp << endl;
    cout << res2_asm << endl;
    assert(fabs(res2_cpp - res2_asm) <= eps);

    return 0;
}
