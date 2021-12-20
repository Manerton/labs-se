#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
using namespace std;

using Row = vector<int64_t>;
using Matrix = vector<Row>;

/// Создать матрицу с размером NxN.
Matrix createMatrix_NxN(size_t N)
{
    Matrix m(N);
    for (auto& row: m)
    {
        row.resize(N);
    }
    return m;
}

void saveMatrixToFile(const Matrix& m, ofstream& file)
{
    for (auto& row: m)
    {
        for (auto e: row)
        {
            file << e << " ";
        }
        file << endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        return 1;
    }

    const string filename = argv[1];
    const size_t size = stoul(argv[2]);

    ofstream file(filename);
    if (file)
    {
        // Читаем матрицы из файла.
        Matrix A = createMatrix_NxN(size);
        Matrix B = createMatrix_NxN(size);

        // Подготовим генератор случайных чисел.
        random_device rd;
        mt19937 mt(rd());

        uniform_int_distribution<int64_t> dist(-100, 100);

        for (auto& row: A)
        {
            generate(row.begin(), row.end(), [&](){ return dist(mt); } );
        }

        for (auto& row: B)
        {
            generate(row.begin(), row.end(), [&](){ return dist(mt); } );
        }

        saveMatrixToFile(A, file);

        file << endl;

        saveMatrixToFile(B, file);
    }
    else
    {
        cout << "Can't open file!" << endl;
        return 1;
    }

    return 0;
}
