#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

// исключение
class invalid_input_exception {};
class zero_divide_exception {};

string toString(const vector<uint8_t> &r) // вектор цифр в строку
{
    stringstream ss;
    if (r[0] == 253) {ss << "-";}
    else ss << int(r[0]);
    size_t N = r.size();
    for (size_t i = 1; i < N; ++i) // int для вывода цифр от 0 до 9, а не символов под кодами от 0 до 9
    {
        ss << int(r[i]);
    }
    return ss.str();
}

void optimize_string(string &s)
{
    size_t N = s.length();
    if (N > 1)
    {
        size_t i = 0;
        if (s[0] == '-') ++i;
        while ((i < N-1) && (s[i] == '0')) ++i;
        if (s[0] == '-') s.erase(1,i-1);
        else s.erase(0,i);
    }
    if (s.length() == 1 && s[0] == '-') s.push_back('0');
}

/// *** ввод вывод *** ///
istream& operator>>(std::istream& t, vector<uint8_t> &r) // ввод (строка в вектор)
{
    string s;
    t >> s;
    optimize_string(s);
    size_t N = s.length();
    r.resize(N);
    for (size_t i = 0; i < N; ++i)
    {
       if ((s[i] >= '0' && s[i] <= '9') || (s[i] == '-' && i == 0)) r[i] = uint8_t(s[i] - '0'); // помещаю в вектор не коды символов аски, а сами цифры от 0 до 9
       else throw invalid_input_exception();
    }
    return t;
}

ostream& operator<<(std::ostream& t, const vector<uint8_t> &r) // вывод вектора цифр, где каждые три цифры отделяю пробелом
{
    string s = toString(r);
    size_t N = s.length();
    for (size_t i = N-3; i != 0 && i < N; i-=3) {s.insert(i, " ");} // i != 0 чтобы не вставить пробел в начало,
    t << s;                                                         // i < N - сработает при переполнении беззнакового size_t,
    return t;                                                       // так как полученное значение будет огромным и > N
}
/// конец ввода вывода ///

/// *** операции сравнения *** ///

bool operator<(const vector<uint8_t> &a, const vector<uint8_t> &b)
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

bool operator>=(const vector<uint8_t> &a, const vector<uint8_t> &b)
{
    return !(a<b);
}

bool operator>(const vector<uint8_t> &a, const vector<uint8_t> &b)
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

bool operator<=(const vector<uint8_t> &a, const vector<uint8_t> &b)
{
    return !(a>b);
}
/// конец операций сравнения ///

vector<uint8_t> toAdditionalCode (const vector<uint8_t> &a, size_t N) // перевод числа в доп код
{
    size_t diff = N - a.size();
    vector<uint8_t> temp(diff,9);
    temp.insert(temp.end(),a.begin(),a.end());
    for (size_t i = diff; i < N; ++i)
    {
        temp[i] = 9 - temp[i];
    }
    temp.back()++;
    return temp;
}

vector<uint8_t>& addition(const vector<uint8_t> &max, const vector<uint8_t> &min, vector<uint8_t> &temp) // прибавляем к большему числу меньшее
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


vector<uint8_t> operator-(const vector<uint8_t> &a, const vector<uint8_t> &b) // вычитание
{
    vector<uint8_t> temp;
    vector<uint8_t> negative_b (b);
    if (negative_b[0] != 253) negative_b.insert(negative_b.begin(),253);
    else negative_b.erase(negative_b.begin());
    temp = a+negative_b;
    return temp;
}

void fraction(const vector<uint8_t> &a, const vector<uint8_t> &b) // деление
{
    if (a == b) cout << "1" << endl;
    bool NF = false; // negative flag
    vector<uint8_t> temp_a = a;
    vector<uint8_t> temp_b = b;
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
        vector<uint8_t> res = {0};
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
        vector<uint8_t> a,b;
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

