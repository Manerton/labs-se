#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

string toString(const vector<uint8_t> &r) // вектор цифр в строку
{
    std::stringstream ss;
    if (r[0] == 253) {ss << "-";}
    else ss << int(r[0]);
    for (auto it = r.begin()+1; it != r.end(); ++it) // int для вывода цифр от 0 до 9, а не символов под кодами от 0 до 9
    {
        ss << int(*it);
    }
    return ss.str();
}

/// *** ввод вывод *** ///
istream& operator>>(std::istream& t, vector<uint8_t> &r) // ввод (строка в вектор)
{ /// сделать проверки на входе ///
    string s;
    t >> s;
    size_t N = s.length();
    r.resize(N);
    for (size_t i = 0; i < N; i++)
    {
       r[i] = uint8_t(s[i]) - '0'; // помещаю в вектор не коды символов аски, а сами цифры от 0 до 9
    }
    return t;
}

ostream& operator<<(std::ostream& t, const vector<uint8_t> &r) // вывод вектора цифр, где каждые три цифры отделяю пробелом
{
    string s = toString(r);
    size_t N = s.length();
    for (size_t i = N-3; N > 3; i-=3) {s.insert(i, " "); N-=3;}
    t << s;
    return t;
}
/// конец ввода вывода ///

/// *** операции сравнения *** ///
bool operator==(const vector<uint8_t> &a, const vector<uint8_t> &b)
{
    return (toString(a) == toString(b));
}
bool operator!=(const vector<uint8_t> &a, const vector<uint8_t> &b)
{
    return !(a == b);
}
bool operator<(const vector<uint8_t> &a, const vector<uint8_t> &b)
{
    if (a[0] == 253 && a[0] != b[0]) return true;
    if (a.size() == b.size()) return (toString(a) < toString(b));
    return (a.size() < b.size());
}
bool operator>=(const vector<uint8_t> &a, const vector<uint8_t> &b)
{
    return !(a<b);
}

bool operator>(const vector<uint8_t> &a, const vector<uint8_t> &b)
{
    if (b[0] == 253 && b[0] != a[0]) return true;
    if (a.size() == b.size()) return (toString(a) > toString(b));
    return (a.size() > b.size());
}

bool operator<=(const vector<uint8_t> &a, const vector<uint8_t> &b)
{
    return !(a>b);
}
/// конец операций сравнения ///

vector<uint8_t> toAdditionalCode (const vector<uint8_t> &a, uint64_t N) // перевод числа в доп код
{
    size_t diff = N - a.size();
    vector<uint8_t> temp(diff,0);
    temp.insert(temp.end(),a.begin(),a.end());
    for (auto it = temp.begin(); it != temp.end(); ++it)
    {
        *it = 9 - *it;
    }
    temp[temp.size()-1] += 1;
    return temp;
}

vector<uint8_t>& addition(const vector<uint8_t> &max, const vector<uint8_t> &min, vector<uint8_t> &temp) // сложение макс + мин
{
    temp.resize(1); // доп элемент - разряд числа который может получится при сумме
    temp.insert(temp.begin()+1,max.begin(),max.end());

    size_t j = min.size() - 1;
    for (size_t i = temp.size() - 1; i > 0; --i)
    {
        if (j+1 > 0) // если есть что прибавлять
        {
            temp[i] += min[j]; // прибавляем
            --j;
        }
        if (temp[i] >= 10) { temp[i] -= 10; temp[i-1] += 1;} // сокращаем
    }
    if (temp[0] == 0) temp.erase(temp.begin()); // если доп элемент не понадобился, удаляем его
    return temp;
}

vector<uint8_t>& addition_with_negative(const vector<uint8_t> &a, const vector<uint8_t> &b, vector<uint8_t> &temp) // сложение с отриц числами (используя доп код)
{
    vector<uint8_t> temp2 = toAdditionalCode(b, a.size()); // переводим число в доп код
    temp = addition(a,temp2,temp); // складываем
    temp.erase(temp.begin()); // удаляем лишний разряд получившийся при сумме с доп кодом

    auto it = temp.begin(); // удаляю лишние нули в начале числа
    while ((*it) == 0 && it != temp.end()-1) it++; // считаю количество этих нулей
    if (it != temp.begin()) temp.erase(temp.begin(),it); // удаляю их
    return temp;
}

vector<uint8_t> operator+(const vector<uint8_t> &a, const vector<uint8_t> &b) // операция сложения
{
    vector<uint8_t> temp;
    if (a[0] != 253 && b[0] == 253) // если складываем полож с отриц числом
    {
        vector<uint8_t> u_b = {b.begin()+1, b.end()}; // убираем знак
        if (u_b > a) // если модуль a < модуля b
        {
            temp = addition_with_negative(u_b,a,temp); // использую обратный порядок, чтобы прибавлять меньшее число (a) к большему (u_b)
            temp.insert(temp.begin(),253);
        }
        else temp = addition_with_negative(a,u_b,temp);
    }
    else if (a[0] == 253 && b[0] != 253) // если складываем отриц с положительным
    {
        vector<uint8_t> u_a = {a.begin()+1, a.end()};
        if (u_a > b)
        {
            temp = addition_with_negative(u_a,b,temp);
            temp.insert(temp.begin(),253);
        }
        else temp = addition_with_negative(b,u_a,temp);
    }
    else if (a[0] == 253 && b[0] == 253)  // Отриц складываем с отриц
    {
        vector<uint8_t> u_a = {a.begin()+1, a.end()};
        vector<uint8_t> u_b = {b.begin()+1, b.end()};
        if (u_b > u_a) temp = addition(u_b,u_a,temp); // чтобы всегда прибавлять меньшее число (a) к большему (b)
        else temp = addition(u_a,u_b,temp);
        temp.insert(temp.begin(),253);
    } else {
        if (b > a) temp = addition(b,a,temp);
        else temp = addition(a,b,temp);
    }
    return temp;
}


vector<uint8_t> operator-(const vector<uint8_t> &a, const vector<uint8_t> &b) // вычитание
{
    vector<uint8_t> temp;
    vector<uint8_t> negative_b (b);
    if (negative_b[0] != 253) negative_b.insert(negative_b.begin(),253);
    else negative_b.erase(negative_b.begin());
    temp = a+negative_b;
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
    cout << "Результат: " << c << endl;
    return 0;
}

