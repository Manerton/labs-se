#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <boost/filesystem.hpp>
#include <boost/process/system.hpp>
using namespace std;

/// Миллисекунды в формате дробного числа.
using d_milliseconds = std::chrono::duration<double, std::chrono::milliseconds::period>;

/// Секунды в формате дробного числа.
using d_seconds = std::chrono::duration<double, std::chrono::seconds::period>;

double calcAverageTime(const vector<double> &timeResults)
{
    double res = 0;

    const size_t size = timeResults.size();

    for (size_t i = 0; i < size; ++i)
    {
        res += timeResults[i];
    }

    res /= double(size);

    return res;
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        return 1;
    }

    const string programName = argv[1];
    const string filename = argv[2];
    const size_t size = stoul(argv[3]);
    const size_t launchCount = stoul(argv[4]);

    try
    {
        boost::filesystem::path p1 = programName;
        boost::filesystem::path p2 = filename;

        vector<uint16_t> threadCounts {1, 2, 4, 6, 8};
        for (auto t: threadCounts)
        {
            vector<double> timeResults;
            timeResults.reserve(launchCount);

            for (size_t i = 0; i < launchCount; ++i)
            {
                auto start = std::chrono::steady_clock::now();

                int result = boost::process::system(p1, p2, to_string(t), to_string(size));

                if (result == -1)
                {
                    throw runtime_error("Calculation error!");
                }

                auto end = std::chrono::steady_clock::now();

                auto time = std::chrono::duration_cast<d_seconds>(end-start).count();
                timeResults.push_back(time);
            }

            double avgTime = calcAverageTime(timeResults);

            cout << t << " | " << avgTime << endl;
        }
    }
    catch(exception& e)
    {
        cout << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
