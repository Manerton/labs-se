#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>

using namespace std;

istream& operator>>(std::istream& t, vector<uint8_t> &r)
{
    string s;
    t >> s;
    move(s.begin(),s.end(),r.begin());
    return t;
}

ostream& operator<<(std::ostream& t, const vector<uint8_t> &r)
{
    for (auto &c : r) { t << int(c); }
    return t;
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №1. Вариант 7.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    vector<uint8_t> a{1,4,5};
    //cin >> a;
    cout << a;
    return 0;
}

