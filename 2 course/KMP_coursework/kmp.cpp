#include <QDebug>
#include <sstream>
#include <iostream>
#include <vector>
#include "kmp.h"
using namespace std;
const vector<pref_func_step_info> &kmp::get_pref_step_info() const
{
    return v_pref_info;
}

const vector<kmp_step_info> &kmp::get_kmp_step_info() const
{
    return v_kmp_info;
}

vector<int> kmp::prefix_function()
{
    size_t N = size_t(substr.length()); // -- длина образца -- //
    vector<int> Pi(N); // -- массив Пи -- //
    Pi[0] = 0;  // -- первый элемент массива всегда равен нулю -- //

    if (N > 1)
    {
        int i = 1;   // -- соответственно, начинаем идти со 2-го символа -- //
        int j = 0;   // -- второй счётчик -- //
        v_pref_info.push_back({Pi[0],i, j,-1});
        while (i < int(N))   // -- пока не закончился образец -- //
        {
            v_pref_info.push_back({-1,i, j,-1});
            if (substr[int(j)] == substr[int(i)]) // -- если символы равны -- //
            {
                Pi[size_t(i)] = j+1;
                v_pref_info.push_back({Pi[size_t(i)],i, j,2});
                ++i;
                ++j;
            }
            else    // -- если символы не равны -- //
            {
                if (j == 0)     // -- и если j = 0 -- //
                {
                    Pi[size_t(i)] = 0;
                    v_pref_info.push_back({Pi[size_t(i)],i, j,0});
                    ++i;
                }
                else    // -- в противном случае -- //
                {
                    j = Pi[size_t(j-1)];
                    v_pref_info.push_back({-1,i, j,1});
                }
            }
        }
    }
    else
    {
        int i = 1;   // -- соответственно, начинаем идти со 2-го символа -- //
        int j = 0;   // -- второй счётчик -- //
        v_pref_info.push_back({Pi[0],i, j,-1});
        v_pref_info.push_back({-1,i, j,-1});
    }

    return Pi;
}

bool kmp::kmp_algorithm(const vector<int> &Pi)
{
    int N = str.length(); // -- длина всей строки -- //
    int M = substr.length(); // -- длина подстроки -- //
    int k = 0;   // -- индекс для строки -- //
    int l = 0;   // -- индекс для подстроки -- //
    v_kmp_info.push_back({l,k,-1});
    v_kmp_info.push_back({l,k,-1});
    while (k < N)   // -- пока не закончился строка -- //
    {
        if (str[k] == substr[l]) // -- если символы равны -- //
        {
            ++k;
            ++l;
            if (l == M) return true;
            v_kmp_info.push_back({l,k,5});
        }
        else    // -- если символы не равны -- //
        {
            if (l == 0)     // -- и если j = 0 -- //
            {
                ++k;
                if (k == N) return false;
                v_kmp_info.push_back({l,k,3});
            }
            else    // -- в противном случае -- //
            {
                l = int(Pi[size_t(l-1)]);
                v_kmp_info.push_back({l,k,4});
            }
        }
    }
    return false;
}

