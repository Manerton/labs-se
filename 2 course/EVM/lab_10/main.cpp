#include <iostream>
#include <cmath>
#include <random>
#include <cassert>
#include <ctime>

using namespace std;

const size_t SIZE = 1000;

void fill_array(float* arr, size_t N, float min, float max)
{
    if (min > max) swap(min, max);
    time_t seed = time(nullptr);
    mt19937 mt(seed);
    uniform_real_distribution<float> dist(min, max);    // значения от мин до макс
    // генерируем числа в заданном диапазоне
    for (size_t i = 0; i < N; ++i) arr[i] = dist(mt);
}

void display_array(float* arr, size_t N)
{
    for (size_t i = 0; i < N; ++i)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

float first_negative_number(float* arr, size_t N)
{
    float res = 0;
    for (size_t i = 0; i < N; ++i)
    {
        if (arr[i] < 0)
        {
            res = arr[i];
            i = N;
        }
    }
    return res;
}

float last_negative_number(float* arr, size_t N)
{
    float res = 0;
    for (size_t i = 0; i < N; ++i)
    {
        if (arr[i] < 0)
        {
            res = arr[i];
        }
    }
    return res;
}


void zadanie_cpp(float* arr, size_t N)
{
    float mult = 2*(first_negative_number(arr,N) + last_negative_number(arr,N));
    for (size_t i = 2; i < N; i+=3)
    {
        arr[i] *= mult;
    }
}

// умножить каждое третье число на удвоенную сумму первого и последнего отрицательных чисел
void zadanie_asm (float* arr, size_t N)
{
    // создаем новый массив, чтобы удобнее с ним было работать из асм вставки, его лучше сделать статическим
    float new_arr[N];
    std::copy(arr, arr + SIZE, new_arr);
    // на третье место ставим нашу удвоенную сумму первого и последнего отрицательных чисел
    float second_arr[4]{1,1,1,1};
    float test = -1;
    // будем передвигаться по три элемента по массиву, чтобы умножать каждый третий элемент
    uint64_t IterationsCount = N / 3;
    float last_neg_num = 0;
    asm __volatile__ (
            R"(
            mov rbx, 0                    # индекс в байтах для массива
            mov rcx, %[N]
            fldz    # для поиска отрицательного элемента
            find_first:
                fld dword ptr %[new_arr][rbx]   # загрузили элемент массива в стек
                fcomi st, st(1)       # выясняем, больше ли этот элемент нуля
                ja not_negative       # если да, то переходим к not_negative
                # иначе
                fstp st(1)            # убираем ноль
                jmp stop_find
                not_negative:
                add rbx, 4            # идем по массиву дальше
                fstp st               # выгружаем из стека элемент массива, который только что загружали
            loop find_first
            stop_find:
                fldz
                mov rbx, 0
                mov rcx, %[N]
            find_last:
                fld dword ptr %[new_arr][rbx]   # загрузили элемент массива в стек
                fcomi st, st(1)       # выясняем, больше ли этот элемент нуля
                ja not_negative2       # если да, то переходим к not_negative
                # иначе
                fst %[last_neg_num]
                not_negative2:
                add rbx, 4            # идем по массиву дальше
                fstp st               # выгружаем из стека элемент массива, который только что загружали
            loop find_last
            fstp st                   # убираем ноль
            fld %[last_neg_num]
            faddp st(1), st
            fadd st, st
            fst %[test]
            fstp dword ptr %[second_arr][8]     # записываем в массив, состоящий из 4-ех элементов

            mov rbx, 0
            mov rcx, %[IterationsCount]   # это количество итераций для цикла при умножении на каждый 3 элемент

            cycle:
                # в SSE элементы читаются по четыре штуки
                movups xmm1, %[new_arr][rbx]    # читаем в xmm1 четыре элемента нашего массива
                mulps xmm1, %[second_arr]       # умножаем, в итоге умножается только третий элемент
                movups %[new_arr][rbx], xmm1    # сохраняем эти четыре элемента в массив обратно
                add rbx,12                      # увеличиваем индекс массива на 12 байтов, чтобы сместится на 3 элемента, чтобы всегда попадать при умножении на нужный нам 3-й элемент
            loop cycle
            )"
            : [new_arr] "+m" (new_arr), [last_neg_num] "+m" (last_neg_num), [test] "+m" (test)
            : [second_arr] "m" (second_arr), [IterationsCount] "m" (IterationsCount), [N] "m" (N)
        );
    cout << endl << test << endl;
    std::copy(new_arr, new_arr + SIZE, arr); // сохраняем результат
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №10. Вариант 7.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    cout << "Умножить каждое третье число на удвоенную сумму первого и последнего отрицательных чисел." << endl;
    const float eps = 0.00001; // -- точность сравнения при ассерте -- //
    float *arr1 = new float[SIZE];
    fill_array(arr1,SIZE,-100,100);
    float *arr2 = new float[SIZE];
    // копируем массив arr1 в arr2
    std::copy(arr1, arr1 + SIZE, arr2);
    cout << "Исходный массив 1 для cpp: " << endl;
    display_array(arr1,SIZE);
    cout << "Исходный массив 2 для asm: " << endl;
    display_array(arr2,SIZE);

    zadanie_cpp(arr1,SIZE);
    zadanie_asm(arr2,SIZE);
    cout << "Массив 1 для cpp: " << endl;
    display_array(arr1,SIZE);
    cout << "Массив 2 для asm: " << endl;
    display_array(arr2,SIZE);

    // -- проверка ассертом содержимого массивов -- //
    for (size_t i = 0; i < SIZE; ++i)
    {
        assert(fabs(arr1[i] - arr2[i]) <= eps);
    }
    return 0;
}
