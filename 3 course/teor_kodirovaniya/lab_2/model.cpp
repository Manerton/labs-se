#include <sstream>
#include <cmath>
#include "model.h"

#include <QDebug>

using namespace std;

double Model::calculate_singleEntropy()
{
    auto size = single_table.rowCount()-1;
    int sum_n = single_table.item(size,st_columns::n_column)->text().toInt();
    for (auto i = 0; i < size; ++i)
    {
        int n = single_table.item(i,st_columns::n_column)->text().toInt();
        double w = double(n)/sum_n;
        singleEntropy += w * log2(w);
    }
    singleEntropy = -singleEntropy;
    return singleEntropy;
}
