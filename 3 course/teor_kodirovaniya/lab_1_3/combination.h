#ifndef COMBINATION_H
#define COMBINATION_H
#include <cstdint>
#include <vector>

class Combination
{
    static inline uint64_t& lookup(const uint64_t k, const uint64_t n)
    {
        return table[N * n + k];
    }

    static inline uint64_t compute(const uint64_t k, const uint64_t n)
    {
        if ((k == 0) || (k == n)) return 1;

        uint64_t v1 = lookup(k - 1,n - 1);
        if (v1 == 0)
        {
            v1 = lookup(k - 1,n - 1) = compute(k - 1,n - 1);
        }
        uint64_t v2 = lookup(k,n - 1);
        if (v2 == 0)
        {
            v2 = lookup(k,n - 1) = compute(k,n - 1);
        }
        return v1 + v2;
    }

    static std::vector<uint64_t> table;
    static uint64_t N;

public:
    Combination() {N = 0;}
    static uint64_t calculate(const uint64_t k, const uint64_t n)
    {
        if (n < k)  return 0;
        if (n == 0) return 0;
        if (k == 0) return 1;
        if (n == k) return 1;
        if (k == 1) return n;
        table.resize(n*n,0);
        N = n;

        uint64_t res = compute(k,n);
        table.clear();
        return res;
    };
};

uint64_t Combination::N = 0;
std::vector<uint64_t> Combination::table;
#endif // COMBINATION_H
