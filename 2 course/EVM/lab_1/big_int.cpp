#include "big_int.h"

void formatstring(string &s) // отформатировать вывод: поставить пробелы каждые 3 цифры
{
    size_t N = s.length();
    N += N / 3; // добавляю место под пробелы
    reverse(s.begin(),s.end()); // переворачиваю строку, чтобы пройтись циклом с конца
    --N; // уменьшаем N на 1 для того, чтобы не вставлять пробел в начало строки перед числами, если в цикле будет i = N-1

    for (size_t i = 3; i < N; i+=3) {s.insert(i, " "); ++i;}

    reverse(s.begin(),s.end()); // переворачиваю обратно
}

string toString(const big_int &r) // преобразую вектор цифр в строку
{
    stringstream ss;
    if (r[0] == sign::minus) {ss << "-";}
    size_t N = r.size();
    for (size_t i = 1; i < N; ++i) // int для вывода цифр от 0 до 9, а не символов под кодами от 0 до 9
    {
        ss << int(r[i]);
    }
    string s = ss.str();
    formatstring(s);
    return s;

}

void optimize_string(string &s) // убираю лишние нули в строке
{
    size_t N = s.length();
    if (N > 2)
    {
        // ищем первый элемент неравный нулю
        auto pos = find_if(s.begin()+1, s.end(),
                                        [](int elem)
                                        {
                                           return (elem != '0');
                                        });
        s.erase(s.begin()+1, pos); // удаляем лишние нули
    }
    if (N == 1 && s[0] == '-') s.push_back('0'); // если введен просто знак минус, добавим 0
}

big_int stobi(const string &str) // string to big integer
{
    big_int r;
    string s = str;
    if (s[0] != '-') s.insert(s.begin(),'+'); // добавляю в начало строки явно знак "+", если первый символ не '-'
    optimize_string(s); // убираем лишние нули из строки
    size_t N = s.length();
    r.resize(N); // делаю размер big_int как у строки

    if (s[0] == '-') r[0] = sign::minus; // ставлю минус
    else if (s[0] == '+') r[0] = sign::plus; // ставлю плюс
    else throw invalid_input_exception(); // если первый символ не равен знаку, то выбрасываю исключение

    for (size_t i = 1; i < N; ++i) // начинаю с единицы, так как знак нам не нужен
    {
       if ( isdigit(s[i]) ) r[i] = uint8_t(s[i] - '0'); // помещаю в вектор не коды символов аски для цифр от 0 до 9, а сами цифры от 0 до 9
       else throw invalid_input_exception(); // выбрасываю исключение, если это не цифра
    }
    return r;
}

/// *** ввод вывод *** ///
std::istream& operator>>(std::istream& t, big_int &r) // ввод (строка в вектор)
{
    string s;
    t >> s;
    r = stobi(s);
    return t;
}

std::ostream& operator<<(std::ostream& t, const big_int &r) // вывод вектора цифр, где каждые три цифры отделяю пробелом
{
    string s = toString(r);
    t << s;
    return t;
}
/// конец ввода вывода ///

/// *** операции сравнения *** ///

bool operator<(const big_int &a, const big_int &b)
{
    // сначала смотрим на знаки
    if (a[0] == sign::minus && b[0] != sign::minus) return true; // если левое число отрицательное, а правое - нет, то true
    if (a[0] != sign::minus && b[0] == sign::minus) return false; // это если наоборот
    if (a == b) return false; // если они равны, то false, оператор == из std::vector

    // если знаки равны, то тогда делаем следующее
    bool answer = false;
    if (a.size() == b.size()) answer = std::operator<(a, b); // использую функцию сравнения из std::vector, если они одинаковой длины
    else answer = a.size() < b.size(); // тут answer будет в зависимости от длин чисел

    if (a[0] == sign::minus && b[0] == sign::minus) answer = !answer; // если числа были отрицательные, инвертирую ответ
    return answer;
}

bool operator>=(const big_int &a, const big_int &b)
{
    return !(a < b);
}

bool operator>(const big_int &a, const big_int &b)
{
    return (b < a); // так как a > b, это b < a
}

bool operator<=(const big_int &a, const big_int &b)
{
    return !(b < a); // тоже реализую через одну операцию <
}
/// конец операций сравнения ///

big_int toAdditionalCode (const big_int &a, size_t N) // перевод числа в доп код
{
    size_t diff = N - a.size();
    big_int temp(1,sign::plus);                    // изначально тут теперь знак плюс
    temp.insert(temp.begin()+1, diff, 9);        // вставляю после знака недостающие девятки
    temp.insert(temp.end(),a.begin()+1,a.end()); // вставляю после девяток изначальные числа

    for (size_t i = diff+1; i < N; ++i)            // преобразую эти числа в соответствии с алгоритмом перевода в доп код
    {
        temp[i] = 9 - temp[i];
    }
    temp.back()++;
    return temp;
}

void addition(const big_int &max, const big_int &min, big_int &result) // прибавляем к большему числу меньшее, результат сохраняется в result
{
    result = max;
    reverse(result.begin(),result.end()); // переворачиваю
    big_int min_reversed = min;
    reverse(min_reversed.begin(),min_reversed.end()); // переворачиваю

    size_t N_min = min.size() - 1; // знак нам не нужен при сложении
    size_t N_max = max.size() - 1;
    size_t j = 0;
    for (size_t i = 0; i < N_max; ++i)
    {
        if (j < N_min) // если есть что прибавлять
        {
            result[i] += min_reversed[j];
            ++j;
        }

        if (result[i] >= 10)
        {
            if (i == N_max - 1) {result.insert(result.end()-1,0);} // добавляю дополнительный разряд в начало который получился при сумме
            result[i] -= 10; ++result[i+1];
        } // сокращаем
    }
    reverse(result.begin(), result.end()); // переворачиваю обратно
}

void addition_with_negative(const big_int &a, const big_int &b, big_int &result) // сложение с отриц числами (используя доп код)
{
    big_int AdditionalCode = toAdditionalCode(b, a.size()); // переводим число в доп код

    addition(a,AdditionalCode,result); // складываем
    result.erase(result.begin()+1); // удаляем лишний разряд получившийся при сумме с доп кодом

    auto it = result.begin()+1; // удаляю лишние нули в начале числа
    while ((*it) == 0 && it != result.end()-1) it++; // считаю количество этих нулей
    if (it != result.begin()+1) result.erase(result.begin()+1,it); // удаляю их, если были
}

big_int operator+(const big_int &a, const big_int &b) // операция сложения
{
    big_int result;
    if (a[0] != sign::minus && b[0] == sign::minus) // если складываем полож с отриц числом
    {
        big_int u_b = b;
        u_b[0] = sign::plus; // меняем знак
        if (u_b > a) // если модуль b > модуля a
        {
            addition_with_negative(u_b,a,result); // использую обратный порядок, чтобы прибавлять меньшее число (a) к большему (u_b)
            result[0] = sign::minus; // знак у результата отрицательный
        }
        else addition_with_negative(a,b,result);

    }
    else if (a[0] == sign::minus && b[0] != sign::minus) // если складываем отриц с полож числом
    {
        big_int u_a = a;
        u_a[0] = sign::plus;
        if (u_a > b)
        {
            addition_with_negative(u_a,b,result);
            result[0] = sign::minus;
        }
        else addition_with_negative(b,u_a,result);
    } else {  // если складываем полож числа или отриц + отриц
        if (b.size() > a.size()) addition(b,a,result);
        else addition(a,b,result);
    }
    return result;
}

void operator++(big_int &a)
{
    size_t N = a.size()-1; // знак нам не нужен при этой операции
    ++a[N];
    reverse(a.begin(),a.end()); // Переворачиваю

    for (size_t i = 0; i < N; ++i)
    {
        if (a[i] >= 10)
        {
            if (i == N - 1) {a.insert(a.end()-1,0);} // добавляю дополнительный разряд в начало который получился при сумме
            a[i] -= 10; ++a[i+1];
        } // сокращаем
    }
    reverse(a.begin(),a.end()); // переворачиваю обратно
}


big_int operator-(const big_int &a, const big_int &b) // вычитание
{
    big_int result;
    big_int negative_b (b);
    if (negative_b[0] != sign::minus) negative_b[0] = sign::minus;
    else negative_b[0] = sign::plus;
    result = a+negative_b;
    return result;
}

void fraction(big_int &a, big_int &b, big_int &res) // деление
{
    if (a == b) res = stobi("1");
    else res = stobi("0");
    bool NFa = false; // negative flag
    bool NFb = false;
    if (a[0] == sign::minus) {a[0] = sign::plus; NFa = true;}
    if (b[0] == sign::minus) {b[0] = sign::plus; NFb = true;}
    if (b.size() == 2 && b[1] == 0) throw zero_divide_exception();
    if (a > b)
    {
        while (a >= b) {a = a - b; ++res;}
    }
    if (res != stobi("0"))
    {
        if ((NFa && !NFb) || (!NFa && NFb)) res[0] = sign::minus;
    } else{
        if ((NFa && !NFb) || (!NFa && NFb)) a[0] = sign::minus;
    }
}
