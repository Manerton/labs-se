#include <sstream>
#include <cmath>
#include "model.h"

#include <QDebug>

using namespace std;

double Model::calculate_singleEntropy(const QTableWidget *table)
{
    double singleEntropy = 0;
    auto size = table->rowCount()-1;
    int sum_n = table->item(size,columns::n_column)->text().toInt();
    for (auto i = 0; i < size; ++i)
    {
        int n = table->item(i,columns::n_column)->text().toInt();
        double w = double(n)/sum_n;
        singleEntropy += w * log2(w);
    }
    singleEntropy = -singleEntropy;
    return singleEntropy;
}

double Model::calculate_uslovnayaEntropy(const QTableWidget *A, const QTableWidget *B)
{
    double uslovnayaEntropy = 0;
    // A - первая таблица (однобуквенные сочетания), B - вторая (двухбуквенные сочетания).
    const auto size_A = A->rowCount()-1;
    const auto size_B = B->rowCount()-1;
    // дельта n в таблице
    const int sum_n_A = A->item(size_A,columns::n_column)->text().toInt();
    const int sum_n_B = B->item(size_B,columns::n_column)->text().toInt();
    // проходим по первой таблице
    for (auto i = 0; i < size_A; ++i)
    {
        // значение X из первой таблицы (символ)
        const QString X_A = A->item(i,columns::X_column)->text();
        // значение n из первой таблицы (количество появлений символа)
        const int n_A = A->item(i,columns::n_column)->text().toInt();
        // высчитываем w (относительная частота появления символа)
        double w_A = double(n_A)/sum_n_A;
        // проходим по второй таблице
        for (auto j = 0; j < size_B; ++j)
        {
            // двубуквенное сочетание из второй таблицы
            QString X_B = B->item(j,columns::X_column)->text();
            if (X_B.left(1) == X_A)
            {
                int n_B = B->item(j,columns::n_column)->text().toInt();
                double w_B = double(n_B)/sum_n_B;
                uslovnayaEntropy += w_B * log2(w_B/w_A);
            }
        }
    }
    uslovnayaEntropy = -uslovnayaEntropy;
    return uslovnayaEntropy;
}

int Model::calculateLengthOfCode(const int m)
{
    return ceil(log2(m));
}

double Model::calculateDp(const double H_A, const int m)
{
    return 1-(H_A/log2(m));
}

double Model::calculateDs(const double H_A, const double H_B_over_A)
{
    return 1-(H_B_over_A/H_A);
}

double Model::calculateD(const double Dp, const double Ds)
{
    return Dp+Ds-Dp*Ds;
}
