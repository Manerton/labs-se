#include <sstream>
#include <cmath>
#include <bitset>
#include <algorithm>
#include <QDebug>
#include <random>
#include "model.h"

using namespace std;

static mt19937 mt(time(0));

int Model::getR() const
{
    return r;
}

int Model::getN() const
{
    return n;
}

Model::Matrix *Model::getCheck_matr_ptr()
{
    return &check_matr;
}

Model::Matrix *Model::getGen_matr_ptr()
{
    return &gen_matr;
}

Model::Row Model::getB() const
{
    return B;
}

Model::Row Model::getB_with_err() const
{
    return B_with_err;
}

Model::Row Model::getS() const
{
    return S;
}

int Model::getError_razryad() const
{
    return error_razryad;
}

void Model::calculate_r()
{
    double val = log2(k+1+log2(k+1));
    r = ceil(val);
    n = k+r;
}

Model::Matrix Model::create_identity_matr(const size_t size) const
{
    Matrix M(size);
    for (size_t i = 0; i < size; ++i)
    {
        M[i].resize(size,0);
        M[i][i] = 1;
    }
    return M;
}

void Model::create_transposed_additional_matr(const size_t rows, const size_t columns)
{
    transposed_additional_matr.resize(rows);
    uint64_t c = 3;
    uint64_t identity_bit = 4;
    for (auto &row : transposed_additional_matr)
    {
        row.resize(columns);
        // если столбец добавочной матрицы равен столбцу единичной матрицы
        if (c == identity_bit)
        {
            identity_bit <<= 1;
            ++c;
        }
        auto newRow = intToBit(c);
        // на тот случай, если число в двоичном представлении короче, чем кол-во элементов в столбце
        // то пишем в обратном порядке с конца столбца, чтобы сохранить ведущие нули
        reverse_copy(newRow.begin(),newRow.end(),row.rbegin());

        ++c;
    }
}

Model::Matrix Model::transpose_matr(const Model::Matrix &M) const
{
    size_t rows = M[0].size();
    size_t columns = M.size();
    Matrix T_M (rows);
    for (size_t i = 0; i < rows; ++i)
    {
        T_M[i].resize(columns);
        for (size_t j = 0; j < columns; ++j)
        {
            T_M[i][j] = M[j][i];
        }
    }
    return T_M;
}

std::vector<Model::value_type> Model::intToBit(const uint64_t c) const
{
    size_t bitLen = 0;
    uint64_t copy_c = c;
    while (copy_c != 0)
    {
        copy_c >>= 1;
        ++bitLen;
    }
    vector<value_type> bitStr(bitLen,0);
    uint64_t m = 1;
    for (size_t i = 0; i < bitLen; ++i)
    {
        bitStr[i] = ((c & (m << i)) ? 1 : 0); // иду с конца числа c и пишу 1 или 0 в зависимости от бита числа на позиции i
    }

    reverse(bitStr.begin(),bitStr.end()); // Так как шел с конца, то переворачиваю вектор
    return bitStr;
}

void Model::create_check_matr()
{
    check_matr.resize(r);
    auto identity_matr = create_identity_matr(r);
    for (int i = 0; i < r; ++i)
    {
        check_matr[i].reserve(n);
        // вставляем добавочную матрицу
        check_matr[i].insert(check_matr[i].begin(),
                             additional_matr[i].begin(),
                             additional_matr[i].end());
        // вставляем единичную матрицу
        check_matr[i].insert(check_matr[i].begin()+k,
                             identity_matr[i].begin(),
                             identity_matr[i].end());
    }
}

void Model::create_gen_matr()
{
    gen_matr.resize(k);
    auto identity_matr = create_identity_matr(k);
    for (int i = 0; i < k; ++i)
    {
        gen_matr[i].reserve(n);
        // вставляем единичную матрицу
        gen_matr[i].insert(gen_matr[i].begin(),
                             identity_matr[i].begin(),
                             identity_matr[i].end());
        // вставляем добавочную матрицу
        gen_matr[i].insert(gen_matr[i].begin()+k,
                             transposed_additional_matr[i].begin(),
                             transposed_additional_matr[i].end());
    }
}

Model::Row Model::mult_matrix_on_vector(const Matrix &M, const Model::Row &V) const
{
    Row result;
    const size_t rows = M.size();
    const size_t columns = M[0].size();
    result.resize(columns);
    for (size_t j = 0; j < columns; ++j)
    {
        for (size_t i = 0; i < rows; ++i)
        {
            result[j] ^= V[i] * M[i][j];
        }
    }
    return result;
}

void Model::add_error_to_B()
{
    B_with_err = B;
    uniform_int_distribution<int> error_dist(0, n-1);
    size_t error_i = error_dist(mt);
    B_with_err[error_i] ^= 1;
}

void Model::find_error_razryad()
{
    for (int j = 0; j < n; ++j)
    {
        int equal_elems = 0;
        for (int i = 0; i < r; ++i)
        {
            equal_elems += (check_matr[i][j] == S[i]);
        }
        if (equal_elems == r)
        {
            error_razryad = j+1;
            j = n;
        }
    }
}
