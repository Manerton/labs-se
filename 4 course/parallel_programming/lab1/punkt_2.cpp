#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;

// float seconds
using f_seconds = duration<float, seconds::period>;

int main()
{
    auto t1 = high_resolution_clock::now();

    double x = 12345.6789;
    for (int i = 0; i < 10000; i++)
    {
        for (int j = 0; j < 10000; j++)
        {
            x = sqrt(x);
            x = x + 0.000000001;
            x = pow(x, 2);
        }
    }
    cout << "A: " << x << endl;

    x = 12345.6789;
    for (int i = 0; i < 10000; i++)
    {
        for (int j = 0; j < 10000; j++)
        {
            x = sqrt(x);
            x = x + 0.000000001;
            x = pow(x, 2);
        }
    }
    cout << "B: " << x << endl;

    x = 12345.6789;
    for (int i = 0; i < 10000; i++)
    {
        for (int j = 0; j < 10000; j++)
        {
            x = sqrt(x);
            x = x + 0.000000001;
            x = pow(x, 2);
        }
    }
    cout << "C: " << x << endl;

    x = 12345.6789;
    for (int i = 0; i < 10000; i++)
    {
        for (int j = 0; j < 10000; j++)
        {
            x = sqrt(x);
            x = x + 0.000000001;
            x = pow(x, 2);
        }
    }
    cout << "D: " << x << endl;

    x = 12345.6789;
    for (int i = 0; i < 10000; i++)
    {
        for (int j = 0; j < 10000; j++)
        {
            x = sqrt(x);
            x = x + 0.000000001;
            x = pow(x, 2);
        }
    }
    cout << "E: " << x << endl;

    x = 12345.6789;
    for (int i = 0; i < 10000; i++)
    {
        for (int j = 0; j < 10000; j++)
        {
            x = sqrt(x);
            x = x + 0.000000001;
            x = pow(x, 2);
        }
    }
    cout << "F: " << x << endl;

    x = 12345.6789;
    for (int i = 0; i < 10000; i++)
    {
        for (int j = 0; j < 10000; j++)
        {
            x = sqrt(x);
            x = x + 0.000000001;
            x = pow(x, 2);
        }
    }
    cout << "G: " << x << endl;

    x = 12345.6789;
    for (int i = 0; i < 10000; i++)
    {
        for (int j = 0; j < 10000; j++)
        {
            x = sqrt(x);
            x = x + 0.000000001;
            x = pow(x, 2);
        }
    }
    cout << "H: " << x << endl;

    x = 12345.6789;
    for (int i = 0; i < 10000; i++)
    {
        for (int j = 0; j < 10000; j++)
        {
            x = sqrt(x);
            x = x + 0.000000001;
            x = pow(x, 2);
        }
    }
    cout << "I: " << x << endl;

    auto t2 = high_resolution_clock::now();
    cout << duration_cast<f_seconds>(t2-t1).count() << endl;

    system("pause");
    return 0;
}
