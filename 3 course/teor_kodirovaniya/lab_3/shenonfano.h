#ifndef SHENONFANO_H
#define SHENONFANO_H
#include <QTableWidget>
#include <vector>
#include <set>
#include <QDebug>

class ShenonFano
{
    using node = std::pair<QChar, int>;
    enum columns : uint8_t
    {
        nomer_column  = 0,
        X_column      = 1,
        w_column      = 2
    };
    QTableWidget *table = nullptr;
    double L = 0;
    std::vector<node> node_array;
    std::map<QChar,QString> shenon_code_map;
    int N = 0; // сумма частот
    // подготовить таблицу для кодирования (шеннона-фано и хаффмана) и вернуть массив с парами: символ - частота появления n
    std::vector<node> prepareTable_coding(const std::multiset<QString> &set);
    // найти дельту сумм вероятностей между двумя группами
    int find_delta_between_groups(const std::vector<int> &n_array);
    // заполнить таблицу в соответствии с алгоритмом шеннона-фано
    void shenonFano_alg(const std::vector<int> &n_array, const int start_row_i);
    void calculate_L();
public:
    ShenonFano(QTableWidget* _table, const std::multiset<QString> &set) : table(_table), node_array{ prepareTable_coding(set) }, N(set.size())
    {
        // выше подготавливаем таблицу и получаем массив пар: символ -  частота n
        std::vector<int> n_array;
        for (auto &val : node_array)
        {
            n_array.push_back(val.second);
        }
        table->insertColumn(3);
        // запускаем алгоритм
        shenonFano_alg(n_array,0);
        int code_column = table->columnCount();
        table->insertColumn(code_column);
        table->setHorizontalHeaderItem(code_column, new QTableWidgetItem("код"));
        int N = table->rowCount();
        for (int i = 0; i < N; ++i)
        {
            QChar X = table->item(i,ShenonFano::columns::X_column)->text()[0];
            table->setItem(i,code_column,new QTableWidgetItem(shenon_code_map[X]));
        }
        calculate_L();
    }
    double getL() const;
};

#endif // SHENONFANO_H
