#ifndef HAFFMAN_H
#define HAFFMAN_H
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <QTableWidget>
#include <QString>
#include <QDebug>

// для работы визуализации хаффмана, вместе с exe программы должна лежать папка graphviz с содержимым папки bin программы graphviz (dot.exe и его dll)
class Haffman
{
    using node = std::pair<QString, int>;
    QTableWidget *table = nullptr;
    std::vector<node> node_array;
    std::multiset<QString>& set;
    int N = 0; // сумма частот
    int step = 0;   // шаг алгоритма
    std::map<QChar,QString> haffman_map;
    double L = 0;
    // методы
    QString createNodeStr(const QString &id, const QString &label, const int p);
    QString addLabel(const QString &id1, const QString &id2, const QString &id12);
    std::vector<QString> text;
    void startText();
    void writeToFile();
    void Haffman_alg(std::vector<node> &node_arr);
    void calculateL();
public: 
    Haffman(QTableWidget *table, std::multiset<QString>& _set) : set(_set), N(_set.size())
    {
        startText();
        for (auto it = set.begin(); it != set.end(); it = set.upper_bound(*it))
        {
            const auto symbol = *it;
            // получаем количество повторений элемента в множестве
            const auto count = set.count(symbol);
            // составляем узлы кодового дерева
            node_array.push_back({symbol,count});
        }

        std::sort(node_array.begin(),node_array.end(),[](const node &a, const node &b){
            return a.second > b.second;
        });
        auto copy_node_arr = node_array;
        Haffman_alg(copy_node_arr);
        writeToFile();
        table->setRowCount(0);
        int i_row = table->rowCount();
        for (auto &val : haffman_map)
        {
            std::reverse(val.second.begin(),val.second.end());
            table->insertRow(i_row);
            table->setItem(i_row,0, new QTableWidgetItem(val.first));
            table->setItem(i_row,1, new QTableWidgetItem(val.second));
            ++i_row;
        }
        calculateL();
    }
    double getL() const;
};

#endif // HAFFMAN_H
