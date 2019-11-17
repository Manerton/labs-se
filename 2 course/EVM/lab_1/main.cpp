#include <iostream>
#include <algorithm>
#include <vector>
#include <cctype>
#include <sstream>
#include <string>

using namespace std;

using big_int = vector<uint8_t>;

// исключение
class invalid_input_exception {}; // некорректный ввод
class zero_divide_exception {}; // деление на ноль

// знаки
enum sign { plus = 11, minus = 12 };

string toString(const big_int &r) // вектор цифр в строку
{
    stringstream ss;
    if (r[0] == sign::minus) {ss << "-";}
    size_t N = r.size();
    for (size_t i = 1; i < N; ++i) // int для вывода цифр от 0 до 9, а не символов под кодами от 0 до 9
    {
        ss << int(r[i]);
    }
    return ss.str();
}

void optimize_string(string &s) // убираю лишние нули в строке
{
    size_t N = s.length();
    if (N > 1)
    {
        // ищем первый элемент неравный нулю
        string::iterator pos = find_if(s.begin()+1, s.end(),
                                        [](int elem){
                                           return (elem != '0');
                                        });
        s.erase(s.begin()+1, pos); // удаляем лишние нули
    }
    if (s.length() == 1 && s[0] == '-') s.push_back('0'); // если введен просто знак минус, добавим 0
    cout << "debug: ";
    cout << s << endl;
}

/// *** ввод вывод *** ///
istream& operator>>(std::istream& t, big_int &r) // ввод (строка в вектор)
{
    string s;
    t >> s;
    if (s[0] != '-') s.insert(s.begin(),'+'); // добавляю в строку явно знак "+", если первый символ не '-'
    optimize_string(s); // убираем лишние нули из строки
    size_t N = s.length();
    r.resize(N); // выделяю сразу память для big_int

    if (s[0] == '-') r[0] = sign::minus; // ставлю минус и увеличиваю счетчик, так как знак '-' явно присутствует в строке
    else if ( isdigit(s[0]) ) r[0] = sign::plus; // ставлю плюс
    else throw invalid_input_exception(); // если первый символ не равен цифре или знаку, то выбрасываем исключение

    for (size_t i = pos; i < N; ++i)
    {
       if ( isdigit(s[i]) ) r[i] = uint8_t(s[i] - '0'); // помещаю в вектор не коды символов аски, а сами цифры от 0 до 9
       else throw invalid_input_exception();
    }
    return t;
}

ostream& operator<<(std::ostream& t, const big_int &r) // вывод вектора цифр, где каждые три цифры отделяю пробелом
{
    string s = toString(r);
    size_t N = s.length();
    reverse(s.begin(),s.end());

    for (size_t i = 2; i < N; i+=3) {s.insert(i, " ");}
    reverse(s.begin(),s.end());
    t << s;
    return t;
    //for (size_t i = N-3; i != 0 && i < N; i-=3) {s.insert(i, " ");}
}
/// конец ввода вывода ///

/// *** операции сравнения *** ///

bool operator<(const big_int &a, const big_int &b)
{
    if (a[0] == 253 && b[0] != 253) return true;
    if (a[0] != 253 && b[0] == 253) return false;
    if (a == b) return false;
    bool answer = false;

    if (a.size() == b.size()) answer = std::operator<(a, b); // иначе рекурсивный вызов
    else answer = a.size() < b.size();

    if (a[0] == 253 && b[0] == 253) answer = !answer;
    return answer;
}

bool operator>=(const big_int &a, const big_int &b)
{
    return !(a<b);
}

bool operator>(const big_int &a, const big_int &b)
{
    if (a[0] == 253 && b[0] != 253) return false;
    if (a[0] != 253 && b[0] == 253) return true;
    if (a == b) return false;
    bool answer = false;

    if (a.size() == b.size()) answer = std::operator>(a, b);
    else answer = a.size() > b.size();

    if (a[0] == 253 && b[0] == 253) answer = !answer;
    return answer;
}

bool operator<=(const big_int &a, const big_int &b)
{
    return !(a>b);
}
/// конец операций сравнения ///

big_int toAdditionalCode (const big_int &a, size_t N) // перевод числа в доп код
{
    size_t diff = N - a.size();
    big_int temp(diff,9);
    temp.insert(temp.end(),a.begin(),a.end());
    for (size_t i = diff; i < N; ++i)
    {
        temp[i] = 9 - temp[i];
    }
    temp.back()++;
    return temp;
}

big_int& addition(const big_int &max, const big_int &min, big_int &temp) // прибавляем к большему числу меньшее
{
    temp = max;
    size_t N_min = min.size();
    size_t N_max = max.size();
    size_t j = N_min-1;
    for (size_t i = N_max - 1; i < N_max; --i)
    {
        if (j < N_min) // если есть что прибавлять
        {
            temp[i] += min[j];
            --j;
        }

        if (temp[i] >= 10)
        {
            if (i == 0) {temp.insert(temp.begin(),0); ++i;} // добавляю дополнительный разряд в начало который получился при сумме
            temp[i] -= 10; temp[i-1]++;
        } // сокращаем
    }
    return temp;
}

big_int& addition_with_negative(const big_int &a, const big_int &b, big_int &temp) // сложение с отриц числами (используя доп код)
{
    big_int temp2 = toAdditionalCode(b, a.size()); // переводим число в доп код
    temp = addition(a,temp2,temp); // складываем
    temp.erase(temp.begin()); // удаляем лишний разряд получившийся при сумме с доп кодом

    auto it = temp.begin(); // удаляю лишние нули в начале числа
    while ((*it) == 0 && it != temp.end()-1) it++; // считаю количество этих нулей
    if (it != temp.begin()) temp.erase(temp.begin(),it); // удаляю их
    return temp;
}

big_int operator+(const big_int &a, const big_int &b) // операция сложения
{
    big_int temp;
    if (a[0] != 253 && b[0] == 253) // если складываем полож с отриц числом
    {
        big_int u_b = {b.begin()+1, b.end()}; // убираем знак
        if (u_b > a) // если модуль a < модуля b
        {
            temp = addition_with_negative(u_b,a,temp); // использую обратный порядок, чтобы прибавлять меньшее число (a) к большему (u_b)
            temp.insert(temp.begin(),253);
        }
        else temp = addition_with_negative(a,u_b,temp);
    }
    else if (a[0] == 253 && b[0] != 253) // если складываем отриц с положительным
    {
        big_int u_a = {a.begin()+1, a.end()};
        if (u_a > b)
        {
            temp = addition_with_negative(u_a,b,temp);
            temp.insert(temp.begin(),253);
        }
        else temp = addition_with_negative(b,u_a,temp);
    }
    else if (a[0] == 253 && b[0] == 253)  // Отриц складываем с отриц
    {
        big_int u_a = {a.begin()+1, a.end()};
        big_int u_b = {b.begin()+1, b.end()};
        if (u_b > u_a) temp = addition(u_b,u_a,temp); // чтобы всегда прибавлять меньшее число (a) к большему (b)
        else temp = addition(u_a,u_b,temp);
        temp.insert(temp.begin(),253);
    } else {
        if (b > a) temp = addition(b,a,temp);
        else temp = addition(a,b,temp);
    }
    return temp;
}

void operator++(vector <uint8_t> &a)
{
    size_t N = a.size();
    ++a.back();
    for (size_t i = N-1; i < N; --i)
    {
        if (a[i] >= 10)
        {
            if (i == 0) {a.insert(a.begin(),0); ++i;}
            a[i] -= 10; ++a[i-1];
        } // сокращаем
    }
}


big_int operator-(const big_int &a, const big_int &b) // вычитание
{
    big_int temp;
    big_int negative_b (b);
    if (negative_b[0] != 253) negative_b.insert(negative_b.begin(),253);
    else negative_b.erase(negative_b.begin());
    temp = a+negative_b;
    return temp;
}

void fraction(const big_int &a, const big_int &b) // деление
{
    if (a == b) cout << "1" << endl;
    bool NF = false; // negative flag
    big_int temp_a = a;
    big_int temp_b = b;
    if (a[0] == 253 || b[0] == 253)
    {
        if ((a[0] == 253 && b[0] != 253) || (a[0] != 253 && b[0] == 253)) NF = true;
        if (a[0] == 253) temp_a.erase(temp_a.begin());
        if (b[0] == 253) temp_b.erase(temp_b.begin());
    }
    if (temp_b.size() == 1 && temp_b[0] == 0) throw zero_divide_exception();
    if (temp_a < temp_b)
    {
        cout << "  " << temp_a << endl;
        size_t count = temp_b.size();
        if (count % 3 != 0) count++;
        count += count/3 - 1;
        if (NF) cout << "- ";
        else cout << "  ";
        cout << string(count,'-') << endl;
        cout << "  " << temp_b << endl;
    }
    if (temp_a > temp_b)
    {
        big_int res = {0};
        while (temp_a >= temp_b) {temp_a = temp_a - temp_b; ++res;}
        if (temp_a[0] == 0) cout << res << endl;
        else {
            size_t res_count = res.size();
            if (res_count % 3 != 0) res_count++;
            res_count += res_count/3 - 1;
            cout << string(res_count+1, ' ') << temp_a << endl;
            size_t count = temp_a.size();
            if (count % 3 != 0) count++;
            count += count/3 - 1;
            if (NF) cout << "-";
            else cout << " ";
            cout << res << " " << string(count,'-') << endl;
            cout << string(res_count+1, ' ') << temp_b << endl;
        }
    }
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №1. Вариант 7.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    try // ищем исключения, которые выбрасываются в блоке try и отправляем их для обработки в блок(и) catch
        {
        big_int a,b;
        cin >> a;
        cout << a << endl;
        cin >> b;
        cout << b << endl;
        cout << "Дробь: " << endl;
        fraction(a,b);
    }
    catch (invalid_input_exception) // некорректный ввод
    {
        cerr << "Ошибка: ввод недопустимых символов. Допустимые символы: 0-9, и знак '-' в начале ввода." << endl;
    }
    catch (zero_divide_exception) // некорректный ввод
    {
        cerr << "Ошибка: деление на ноль." << endl;
    }
    return 0;
}

