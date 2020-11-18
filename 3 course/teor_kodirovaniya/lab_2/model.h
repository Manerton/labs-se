#ifndef MODEL_H
#define MODEL_H
#include <cstdint>
#include <QTableWidget>

class Model
{
public:
    enum columns : uint8_t
    {
        nomer_column  = 0,
        X_column      = 1,
        n_column      = 2,
        W_column      = 3
    };

    Model() = default;
    double calculate_singleEntropy(const QTableWidget *table);
    double calculate_uslovnayaEntropy(const QTableWidget *singleTable, const QTableWidget *doubleTable);
    int calculateLengthOfCode(const int m);
    double calculateDp(const double H_A, const int m);
    double calculateDs(const double H_A, const double H_B_over_A);
    double calculateD(const double Dp, const double Ds);
};

#endif // MODEL_H
