#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using chislo = vector<int>;

bool NextChislo(chislo &a, size_t N)
{
    reverse(a.begin(),a.end()); // -- перевернем наше число для удобства -- //
    size_t i = 0;
    size_t last_index_of_a = a.size()-1;
    // -- смотрим последнюю цифру числа (мы перевернули его, поэтому индекс начинается с 0) -- //
    // -- если она уже равна N, т.е в последнем разряде уже максимальное число, то переходим к предыдущему разряду (к предпоследнему), и так далее -- //
    while (i <= last_index_of_a && a[i] == int(N)) ++i;

    if (i > last_index_of_a)
    {
        return false;  // -- если у нас уже все разряды состоят из макс цифры, значит прекращаем генерировать числа -- //
    }

    //if (a[i] >= N)
    //    i--;

    a[i]++;

    if (i != 0) // -- если мы не на последнем разряде -- //
    {
        // -- возвращаемся с нашего разряда на предыдущие, и ставим там 1 -- //
        // -- например у нас было N = 3, M = 3, после 113 должно быть 121, после 122, 123, 131, 132 и так далее -- //
        // -- для этого, когда попадаем не на последний разряд, возвращаемся на предыдущие и ставим там 1 -- //
        for (size_t j = 0; j < i; j++)
        {
            a[j] = 1;
        }
    }

    reverse(a.begin(), a.end());
    return true;
}
void Print(const vector<chislo>& matrix)
{
    for (size_t i = 0; i < matrix.size(); ++i)
    {
        cout.width(3);
        cout << (i+1) << ":  ";
        for (auto var : matrix[i])
        {
            cout << var;
        }
        cout << endl;
    }
}

bool check_for_even_integer(int c)
{
    return (c % 2 == 0);
}

bool check_for_even_repetition(const chislo& a)
{
    auto size = a.size();
    for (size_t i = 0; i < size; ++i)
    {
        if (check_for_even_integer(a[i]))
        {
            if (i+1 != size)
            {
                if (check_for_even_integer(a[i+1])) return true;
            }
        }
    }
    return false;
}

int Solution(const vector<chislo>& matrix)
{
    int res = 0;
    for (const chislo &val : matrix)
    {
        if (check_for_even_repetition(val))
        {
            ++res;
            for (auto cifra : val)
            {
                cout << cifra;
            }
            cout << endl;
        }
    }
    return res;
}
int main()
{
    size_t N, M; // N - Сколько цифр, M - сколько разрядов

    cout << "N = ";
    cin >> N;
    cout << "M = ";
    cin >> M;

    vector<chislo> matrix;
    chislo a(M,1);
    matrix.push_back(a);
    while (NextChislo(a, N))
    {
        matrix.push_back(a);
    }

    Print(matrix);
    cout << Solution(matrix);
    return 0;
}
