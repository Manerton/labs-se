#include <iostream>
#include <fstream>
#include <list>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cassert>

using namespace std;

using Row = vector<int64_t>;
using Matrix = vector<Row>;
using Stripe = Matrix;

struct TaskResult
{
    Stripe calcResult{};            //< Полоса - результат вычисления.

    // Физически полоса со столбцами матрицы B хранится как полоса со строками,
    // но логически - это полоса со столбцами. Этот факт необходимо учитывать
    // при вычислении скалярного умножения, транспонируя полосу
    // при помощи изменения порядка индексов.
    Stripe stripeForSiblingTask;

    size_t beginColumnIndex{};      //< Индекс первого столбца для вычисления.
    size_t endColumnIndex{};        //< Индекс столбца следующий за последним (как в STL).
};

void displayMatrix(const Matrix &m)
{
    for (const auto& row: m)
    {
        for (const auto e: row)
        {
            cout << e << " ";
        }
        cout << endl;
    }
}

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

/**
 * Последовательное умножение двух квадратных матриц A и B.
 * @return Результирующая матрица.
 */
Matrix seqAlg(const Matrix &A, const Matrix &B)
{
    const size_t rowCountA = A.size();
    const size_t columnCountB = rowCountA;
    const size_t rowCountB = B.size();

    // Результирующая матрица C.
    Matrix C { createMatrix_NxN(rowCountA) };

    // Последовательный алгоритм.
    for (size_t i = 0; i < rowCountA; ++i)
    {
        for (size_t j = 0; j < columnCountB; ++j)
        {
            C[i][j] = A[i][0] * B[0][j];
            for (size_t k = 1; k < rowCountB; ++k)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

/**
 * Последовательное умножение двух квадратных матриц A и B.
 * При вычислении транспонировать матрицу B (путем индексов, физически матрица B не транспонируется).
 * @return Результирующая матрица.
 */
Matrix seqAlgWithTransposedB(const Matrix &A, const Matrix &B)
{
    const size_t rowCountA = A.size();
    const size_t columnCountB = B.front().size();
    const size_t rowCountB = rowCountA;

    // Результирующая матрица C.
    Matrix C { createMatrix_NxN(rowCountA) };

    // Последовательный алгоритм.
    for (size_t i = 0; i < rowCountA; ++i)
    {
        for (size_t j = 0; j < rowCountB; ++j)
        {
            C[i][j] = A[i][0] * B[j][0];
            for (size_t k = 1; k < columnCountB; ++k)
            {
                C[i][j] += A[i][k] * B[j][k];
            }
        }
    }

    return C;
}

TaskResult threadCalc(const Stripe& a, const Stripe b, const size_t beginColumnIndex, const size_t endColumnIndex)
{
    // Так как полоса столбцов b физически представлена в виде
    // полосы строк, то будем умножать с учетом этого факта
    // транспонируя полосу b при вычислении при помощи изменения порядка индексов.
    Stripe c { seqAlgWithTransposedB(a, b) };
    return TaskResult{c, b, beginColumnIndex, endColumnIndex};
}

Row copyColumnFromMatrix(const Matrix &m, const size_t columnIndex)
{
    const size_t rowCount = m.size();

    Row result(rowCount);

    for (size_t i = 0; i < rowCount; ++i)
    {
        result[i] = m[i][columnIndex];
    }

    return result;
}

/**
 * Параллельное умножение двух квадратных матриц A и B.
 * @return Результирующая матрица.
 */
Matrix parAlg(const uint16_t threadCount, const Matrix &A, const Matrix &B)
{
    // Размер матриц.
    const size_t size = A.size();
    // Количество шагов алгоритма.
    const auto steps = min(size_t(threadCount), size);
    // Размер матриц должен быть кратным количеству шагов.
    assert (size % steps == 0);
    // Размер полос.
    const size_t stripeSize = size / steps;

    // Результирующая матрица C.
    Matrix C { createMatrix_NxN(size) };

    vector<thread> threads(steps);

    // Лента для имитиции ленточной организации данных.
    vector<TaskResult> tape(steps);

    // В ленту для каждой подзадачи i в соседнюю ячейку ленты i-1
    // (куда соседняя подзадача i-1 помещает свою полосу со столбцами B после итерации алгоритма)
    // поместим i-ую полосу со столбцами матрицы B.
    // Таким образом, в начале вычислений в каждой подзадаче i располагается i-ая полоса со столбцами матрицы B.
    for (size_t i = 0; i < steps; ++i)
    {
        ssize_t siblingIndex = ssize_t(i) - 1;
        // Так как кольцевая структура.
        if (siblingIndex < 0)
        {
            siblingIndex = ssize_t(steps - 1);
        }

        // Выделяем место под stripeSize столбцов в полосе.
        tape[size_t(siblingIndex)].stripeForSiblingTask.reserve(stripeSize);

        const size_t beginColumnB = i * stripeSize;
        const size_t endColumnB = (i * stripeSize) + stripeSize;

        // Сохраняем полосу со столбцами в ячейку ленты.
        for (size_t j = beginColumnB; j < endColumnB; ++j)
        {
            tape[size_t(siblingIndex)].stripeForSiblingTask.emplace_back(copyColumnFromMatrix(B, j));
        }

        // Сохраняем индексы диапазона столбцов (полосы) матрицы B.
        tape[size_t(siblingIndex)].beginColumnIndex = beginColumnB;
        tape[size_t(siblingIndex)].endColumnIndex = endColumnB;
    }

    // Параллельный алгоритм.
    for (size_t step = 0; step < steps; ++step)
    {
        // Берём копию ленты чтобы не было гонок данных, которая может возникнуть
        // при попытке потока сохранить новые данные в ленту
        // пока другой поток пытается считать данные из ленты по тому же индексу.
        auto oldTape = tape;
        // Обойдём все потоки.
        for (size_t i = 0; i < steps; ++i)
        {
            // Индекс ячейки ленты соседней подзадачи.
            ssize_t siblingIndex = ssize_t(i) - 1;
            // Так как кольцевая структура.
            if (siblingIndex < 0)
            {
                siblingIndex = ssize_t(steps - 1);
            }

            // Полоса переданная от соседней подзадачи.
            const Stripe b = oldTape[size_t(siblingIndex)].stripeForSiblingTask;
            // Индексы столбцов матрицы B.
            const size_t beginColumnIndex = oldTape[size_t(siblingIndex)].beginColumnIndex;
            const size_t endColumnIndex = oldTape[size_t(siblingIndex)].endColumnIndex;

            // Подзадача (отдельный поток выполнения).
            thread t ([&A, b, i, stripeSize, beginColumnIndex, endColumnIndex, &tape]() {

                const size_t beginRowA = i * stripeSize;
                const size_t endRowA = (i * stripeSize) + stripeSize;

                Stripe a;
                a.reserve(stripeSize);

                // Получаем полосу матрицы A.
                for (size_t j = beginRowA; j < endRowA; ++j)
                {
                    a.emplace_back(A[j]);
                }

                tape[i] = threadCalc(a, b, beginColumnIndex, endColumnIndex);
            });
            threads[i] = move(t);
        }

        // Ожидаем выполнение всех потоков.
        for (auto& t: threads)
        {
            t.join();
        }

        // Сохраним результаты вычислений каждой подзадачи.
        for (size_t task = 0; task < steps; ++task)
        {
            const size_t beginRow = task * stripeSize;
            const size_t endRow = (task * stripeSize) + stripeSize;

            for (size_t i = beginRow; i < endRow; ++i)
            {
                const size_t beginColumn = tape[task].beginColumnIndex;
                const size_t endColumn = tape[task].endColumnIndex;

                for (size_t j = beginColumn; j < endColumn; ++j)
                {
                    const size_t i_ForResultStripe = i - beginRow;
                    const size_t j_ForResultStripe = j - beginColumn;
                    C[i][j] = tape[task].calcResult[i_ForResultStripe][j_ForResultStripe];
                }
            }
        }
    }

    return C;
}

Matrix loadMatrixFromFile(const size_t size, ifstream& file)
{
    // Создаем матрицу NxN.
    Matrix m { createMatrix_NxN(size) };

    // Читаем по строке из файла.
    string line;

    // Загружаем матрицу m.
    for (size_t i = 0; i < size; ++i)
    {
        getline(file, line);

        // Если прочитали пустую строку.
        if (line.empty())
        {
            getline(file, line);
        }

        stringstream ss(line);
        for (size_t j = 0; j < size; ++j)
        {
            ss >> m[i][j];
        }
    }

    return m;
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        return 1;
    }

    const string filename = argv[1];
    const auto threadCount = stoul(argv[2]);
    const size_t size = stoul(argv[3]);

    ifstream file(filename);
    if (file)
    {
        // Читаем матрицы из файла.
        const Matrix A = loadMatrixFromFile(size, file);
        const Matrix B = loadMatrixFromFile(size, file);

        Matrix C;

        // Умножаем матрицы.
        if (threadCount == 1)
        {
            C = seqAlg(A, B);
        }
        else
        {
            C = parAlg(uint16_t(threadCount), A, B);
        }

        displayMatrix(C);
    }
    else
    {
        cout << "Can't open file!" << endl;
        return 1;
    }

    return 0;
}
