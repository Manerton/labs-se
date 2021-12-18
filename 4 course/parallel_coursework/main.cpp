#include <iostream>
#include <fstream>
#include <list>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

using Row = vector<int64_t>;
using Matrix = vector<Row>;

struct TaskResult
{
    int64_t calcResult{};     //< Результат вычисления.
    size_t columnIndex{};     //< Индекс столбца для вычисления.
    Row rowForSiblingTask;  //< Столбец матрицы B для следующего потока.
};

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
    size_t size = A.size();

    // Результирующая матрица C.
    Matrix C { createMatrix_NxN(size) };

    // Последовательный алгоритм.
    for (size_t i = 0; i < size; ++i)
    {
        for (size_t j = 0; j < size; ++j)
        {
            C[i][j] = A[i][0] * B[0][j];
            for (size_t k = 1; k < size; ++k)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

TaskResult threadCalc(const Row& a, const Row b, const size_t columnIndex)
{
    size_t size = a.size();

    int64_t c = a[0] * b[0];
    for (size_t k = 1; k < size; ++k)
    {
        c += a[k] * b[k];
    }

    return TaskResult{c, columnIndex, b};
}

/**
 * Параллельное умножение двух квадратных матриц A и B.
 * @return Результирующая матрица.
 */
Matrix parAlg(const uint16_t threadCount, const Matrix &A, const Matrix &B)
{
    size_t size = A.size();
    auto steps = min(size_t(threadCount), size);

    // Результирующая матрица C.
    Matrix C { createMatrix_NxN(size) };

    vector<thread> threads(steps);

    // Лента для имитиции ленточной организации данных.
    vector<TaskResult> tape(steps);

    // В ленту для каждой подзадачи i в соседнюю ячейку ленты i-1
    // (куда соседняя подзадача i-1 помещает свой столбец после итерации алгоритма)
    // поместим i-й столбец матрицы B.
    // Таким образом, в начале вычислений в каждой подзадаче i располагается i-й столбец матрицы B.
    for (size_t i = 0; i < steps; ++i)
    {
        ssize_t siblingIndex = ssize_t(i) - 1;
        // Так как кольцевая структура.
        if (siblingIndex < 0)
        {
            siblingIndex = ssize_t(steps - 1);
        }

        // Выделяем место под steps элементов.
        tape[size_t(siblingIndex)].rowForSiblingTask.resize(steps);
        for (size_t j = 0; j < steps; ++j)
        {
            // Сохраняем столбец в ячейку ленты.
            tape[size_t(siblingIndex)].rowForSiblingTask[j] = B[j][i];
        }

        // Сохраняем индекс столбца матрицы B.
        tape[size_t(siblingIndex)].columnIndex = i;
    }

    // Параллельный алгоритм.
    for (size_t step = 0; step < steps; ++step)
    {
        // Берём копию ленты чтобы не было гонок данных
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

            // Столбец переданный от соседней подзадачи.
            const Row b = oldTape[size_t(siblingIndex)].rowForSiblingTask;
            // Индекс столбца матрицы B.
            size_t columnIndex = oldTape[size_t(siblingIndex)].columnIndex;

            // Подзадача (отдельный поток выполнения).
            thread t ([&A, b, i, columnIndex, &tape]() { tape[i] = threadCalc(A[i], b, columnIndex); });
            threads[i] = move(t);
        }

        // Ожидаем выполнение всех потоков.
        for (auto& t: threads)
        {
            t.join();
        }

        // Сохраним результаты вычислений каждой подзадачи.
        for (size_t i = 0; i < steps; ++i)
        {
            C[i][tape[i].columnIndex] = tape[i].calcResult;
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

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        return 1;
    }

    const size_t size = stoul(argv[1]);
    const string filename = argv[2];

    ifstream file(filename);
    if (file)
    {
        // Читаем матрицы из файла.
        const Matrix A = loadMatrixFromFile(size, file);
        const Matrix B = loadMatrixFromFile(size, file);

        // Умножаем матрицы.
        const Matrix C = parAlg(8, A, B);
        displayMatrix(C);
    }
    else
    {
        cout << "Can't open file!" << endl;
        return 1;
    }

    return 0;
}
