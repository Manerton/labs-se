#include <iostream>
#include <cfloat>
#include <cmath>
#include <bitset>

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    uint64_t a = -8192;
    cout << std::bitset<64>(a) << endl;
    uint32_t b = a;
    cout << std::bitset<32>(b) << endl;
    return 0;
}
