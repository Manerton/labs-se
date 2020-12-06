#ifndef MODEL_H
#define MODEL_H
#include <cstdint>
#include <vector>
#include <string>

class Model
{
public:
    // для матриц
    using value_type = char;             // элемент матрицы
    using Row = std::vector<value_type>; // строка
    using Matrix = std::vector<Row>;     // матрица

private:
    // альфа, бета, и бета с ошибкой
    Row A;
    Row B;
    Row B_with_err;
    Row S;
    // число информационных разрядов
    int k = 0;
    // число корректирующих разрядов
    int r = 0;
    int n = 0;
    // номер разряда с ошибкой
    int error_razryad = 0;
    // проверочная и порождающая матрица
    Matrix check_matr;
    Matrix gen_matr;
    Matrix additional_matr;
    Matrix transposed_additional_matr;

    // методы
    void calculate_r(); // вычислить r и n
    // создать единичную матрицу с размером size
    Matrix create_identity_matr(const size_t size) const;
    // создать транспонированную добавочную матрицу для порождающей матрицы
    void create_transposed_additional_matr(const size_t rows, const size_t columns);
    // транспонировать матрицу
    Matrix transpose_matr(const Matrix &M) const;
    // перевести число в битовую строку
    Row intToBit(const uint64_t c) const;
    // создать проверочную матрицу
    void create_check_matr();
    // создать порождающую матрицу
    void create_gen_matr();
    // умножение матрицы на вектор
    Row mult_matrix_on_vector(const Matrix &M, const Row& V) const;
    void add_error_to_B();
    void find_error_razryad();
public:
    Model(Row _a) : A(_a), k(_a.size()) {
        calculate_r();
        create_transposed_additional_matr(k,r);
        // создаем добавочную матрицу для проверочной матрицы (транспонируя предыдущую матрицу)
        additional_matr = transpose_matr(transposed_additional_matr);
        create_check_matr();
        create_gen_matr();
        B = mult_matrix_on_vector(gen_matr,A);
        add_error_to_B();
        auto T_check_matr = transpose_matr(check_matr);
        S = mult_matrix_on_vector(T_check_matr,B_with_err);
        find_error_razryad();
    }

    int getR() const;
    int getN() const;
    Matrix* getCheck_matr_ptr();
    Matrix* getGen_matr_ptr();
    Row getB() const;
    Row getB_with_err() const;
    Row getS() const;
    int getError_razryad() const;
};

#endif // MODEL_H
