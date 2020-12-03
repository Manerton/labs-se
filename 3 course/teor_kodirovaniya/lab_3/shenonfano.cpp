#include "shenonfano.h"
#include <vector>
#include <QString>
#include <map>
using namespace std;

double ShenonFano::getL() const
{
    return L;
}

vector<ShenonFano::node> ShenonFano::prepareTable_coding(const std::multiset<QString> &set)
{
    // очищаем старую таблицу (если была)
    table->setRowCount(0);
    table->setColumnCount(3);
    size_t row_index = 0;
    vector<node> node_array;  // вектор с парой: символ - n (частота) этого символа
    // константы
    const size_t N = set.size();
    const auto nomer_column = ShenonFano::columns::nomer_column;
    const auto X_column     = ShenonFano::columns::X_column;
    const auto W_column     = ShenonFano::columns::w_column;
    // проходим по уникальным элементам
    for (auto it = set.begin(); it != set.end(); it = set.upper_bound(*it))
    {
        const auto symbol = *it;
        // получаем количество повторений элемента в множестве
        const auto count = set.count(symbol);
        table->insertRow(row_index);
        table->setItem(row_index,X_column,new QTableWidgetItem((symbol == " ") ? "пробел" : QString(symbol)));
        table->setItem(row_index,W_column,new QTableWidgetItem(QString::number(count) + " / " + QString::number(N)));
        node_array.push_back({symbol[0],count});
        ++row_index;
    }
    // сортируем в порядке убывания вероятностей
    table->sortItems(W_column,Qt::DescendingOrder);
    const size_t row_count = table->rowCount();
    for (size_t i = 0; i < row_count; ++i)
    {
        table->setItem(i,nomer_column,new QTableWidgetItem(QString::number(i+1)));
    }
    // отсортируем частоты по убыванию
    std::sort(node_array.begin(),node_array.end(),[](const node &a, const node &b){
            return a.second > b.second;
    });
    return node_array;
}

int ShenonFano::find_delta_between_groups(const vector<int> &n_array)
{
    // сумма внутри группы
    const int n_array_sum = accumulate(n_array.begin(),n_array.end(),0);
    // разница между суммами вероятностей двух групп
    // изначально между первым символом и группой из оставшихся символов
    int delta = abs(n_array[0] - (n_array_sum-n_array[0]));
    // индекс элемента - границы первой группы (включительно)
    size_t delta_i = 0;
    const size_t N = n_array.size();
    for (size_t i = 1; i < N; ++i)
    {
        int sum_first_group = 0;
        for (size_t j = 0; j <= i; ++j)
        {
            sum_first_group += n_array[j];
        }
        int new_delta = abs(sum_first_group - (n_array_sum-sum_first_group));

        if (new_delta < delta)
        {
            delta = new_delta;
            delta_i = i;
        }
    }
    return delta_i;
}

void ShenonFano::shenonFano_alg(const std::vector<int> &n_array, const int start_row_i)
{
    int delta_i = find_delta_between_groups(n_array);
    int column_i = table->columnCount()-1;
    table->setHorizontalHeaderItem(column_i, new QTableWidgetItem(""));
    const int X_column = 1;
    const size_t N = n_array.size();
    // объединяю ячейки, чтобы получилось две группы
    size_t first_group_end = start_row_i+delta_i;
    if (delta_i+1 > 1)
    {
        table->setSpan(start_row_i,column_i,delta_i+1,1);
    }
    if ( N-(delta_i+1) > 1 )
    {
        table->setSpan(first_group_end+1,column_i,N-(delta_i+1),1);
    }
    table->setItem(start_row_i,column_i,new QTableWidgetItem("0"));
    for (size_t i = start_row_i; i <= first_group_end; ++i)
    {
        QChar X = table->item(i,X_column)->text()[0];
        shenon_code_map[X].append("0");
    }
    table->setItem(first_group_end+1,column_i,new QTableWidgetItem("1"));
    for (size_t i = first_group_end+1; i < start_row_i+N; ++i)
    {
        QChar X = table->item(i,X_column)->text()[0];
        shenon_code_map[X].append("1");
    }
    // рекурсивно вызываю эту функцию для каждой из подгрупп
    vector<int> first_n_array(n_array.begin(),n_array.begin()+delta_i+1);
    if (first_n_array.size() > 1)
    {
        table->insertColumn(column_i+1);
        shenonFano_alg(first_n_array,start_row_i);
    }
    vector<int> second_n_array(n_array.begin()+delta_i+1,n_array.end());
    if (second_n_array.size() > 1)
    {
        // если нового столбца не было добавлено (так как первая группа состоит из одного элемента), то добавляем здесь
        if (column_i+1 == table->columnCount()) table->insertColumn(column_i+1);
        shenonFano_alg(second_n_array,(first_group_end+1));
    }
}

void ShenonFano::calculate_L()
{
    for (auto it = node_array.begin(); it != node_array.end(); ++it)
    {
        QChar symbol = it->first;
        double w = double(it->second) / N;
        int len = shenon_code_map[symbol].length();
        L += w * len;
    }
}
