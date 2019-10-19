#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

string toString(const vector<uint8_t> &r) // вектор цифр в строку
{
    std::stringstream ss;
    if (r[0] == 253) {ss << "-";}
    else  ss << int(r[0]);
    for (auto it = r.begin()+1; it != r.end(); ++it) // int для вывода цифр от 0 до 9, а не символов под кодами от 0 до 9
    {
        ss << int(*it);
    }
    return ss.str();
}

istream& operator>>(std::istream& t, vector<uint8_t> &r) // ввод (строка в вектор)
{ /// сделать проверки на входе ///
    string s;
    t >> s;
    uint64_t N = s.length();
    r.resize(N);
    for (uint64_t i = 0; i < N; i++)
    {
       r[i] = uint8_t(s[i]) - '0'; // помещаю в вектор не коды символов аски, а сами цифры от 0 до 9
    }
    return t;
}

ostream& operator<<(std::ostream& t, const vector<uint8_t> &r) // вывод вектора цифр, где каждые три цифры отделяю пробелом
{
    string s = toString(r);
    for (auto it = s.end() - 3; it >= s.begin(); it -= 3) {s.insert(it,' ');}
    t << s;
    return t;
}

vector<uint8_t> operator+(const vector<uint8_t> &a, const vector<uint8_t> &b) // сложение
{
    /// проработать сумму с отриц числами ///
    const vector<uint8_t> *max = &a;
    const vector<uint8_t> *min = &b;
    if (a.size() < b.size()) swap(max,min); // для того, чтобы к наибольшему числу прибавлять наименьшее
    vector<uint8_t> temp(1); // создаю вектор на 1 элемент больше
    temp.insert(temp.begin()+1,max->begin(),max->end());

    uint64_t j = min->size() - 1;
    for (uint64_t i = temp.size() - 1; i > 0; --i)
    {
        if (temp[i] >= 10) { temp[i] -= 10; temp[i-1] += 1;}
        if (j+1 > 0) // если есть что прибавлять
        {
            uint8_t t = temp[i] + (*min)[j];
            if (t >= 10) { t -= 10; temp[i-1] += 1;}
            temp[i] = uint8_t(t);
            --j;
        }
    }
    return temp;
}

vector<uint8_t> toAdditionalCode (const vector<uint8_t> &a, uint64_t N)
{
    /// доделать ///
    uint64_t diff = N - a.size();
    vector<uint8_t> temp(diff,0);
    temp.insert(temp.begin()+diff,a.begin(),a.end());
    for (auto it = temp.begin(); it != temp.end(); ++it)
    {
        *it = 9 - *it;
    }
    temp[temp.size()-1] += 1;
    return temp;
}

vector<uint8_t> operator-(const vector<uint8_t> &a, const vector<uint8_t> &b) // вычитание
{
    /// проверить min max и все такое по аналогии с + ///
    vector<uint8_t> temp = a;
    vector<uint8_t> temp2 = toAdditionalCode(b, a.size());
    temp = temp + temp2;
    temp = {temp.begin()+1, temp.end()};
    return temp;
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №1. Вариант 7.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    vector<uint8_t> a,b,c;
    cin >> a;
    cout << a << endl;
    cin >> b;
    cout << b << endl;
    c = a-b;
    cout << c;
    return 0;
}

