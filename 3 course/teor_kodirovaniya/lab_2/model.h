#ifndef MODEL_H
#define MODEL_H
#include <QTableWidget>

class Model
{
    QTableWidget &single_table;  // ссылка на таблицу с вероятностями появления символов в сообщении
    enum st_columns : uint8_t
    {
        nomer_column  = 0,
        X_column      = 1,
        n_column      = 2,
        W_column      = 3
    };
    double singleEntropy = 0;
public:
    Model(QTableWidget* _st) : single_table(*_st) {}
    double calculate_singleEntropy();
};

#endif // MODEL_H
