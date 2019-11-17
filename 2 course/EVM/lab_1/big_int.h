#ifndef BIG_INT_H
#define BIG_INT_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <cctype>
#include <sstream>
#include <string>

using std::vector;
using std::string;
using std::stringstream;
using big_int = vector<uint8_t>;

// исключения
class invalid_input_exception {}; // некорректный ввод
class zero_divide_exception {}; // деление на ноль

// знаки
enum sign { plus = 11, minus = 12 };

void formatstring(string &s);
string toString(const big_int &r);
void optimize_string(string &s);
big_int stobi(const string &str);
std::istream& operator>>(std::istream& t, big_int &r);
std::ostream& operator<<(std::ostream& t, const big_int &r);
bool operator<(const big_int &a, const big_int &b);
bool operator>=(const big_int &a, const big_int &b);
bool operator>(const big_int &a, const big_int &b);
bool operator<=(const big_int &a, const big_int &b);
big_int toAdditionalCode (const big_int &a, size_t N);
void addition(const big_int &max, const big_int &min, big_int &result);
void addition_with_negative(const big_int &a, const big_int &b, big_int &result);
big_int operator+(const big_int &a, const big_int &b);
void operator++(big_int &a);
big_int operator-(const big_int &a, const big_int &b);
void fraction(big_int &a, big_int &b, big_int &res);

#endif // BIG_INT_H
