#include "haffman.h"
#include <fstream>
using namespace std;

double Haffman::getL() const
{
    return L;
}

QString Haffman::createNodeStr(const QString &id, const QString &label, const int p)
{
    QString res;
    QTextStream ss(&res);
    ss << "node" << id << "[label=\"" << label << "|" << p << "/" << N << "\"];" << Qt::endl;
    return res;
}

QString Haffman::addLabel(const QString &id1, const QString &id2, const QString &id12)
{
    QString res;
    QTextStream ss(&res);
    ss << "node" << id1 << "->" << "node" << id12 << "[label=1];"<< Qt::endl;
    ss << "node" << id2 << "->" << "node" << id12 << "[label=0];"<< Qt::endl;
    return res;
}

void Haffman::startText()
{
    QString s = R"(digraph g {
graph [
    rankdir = "LR";
    newrank=true;
];)";
    text.push_back(s);

    s = R"(node [
    fontsize = "16";
    shape = "Mrecord";
];)";
    text.push_back(s);
}

void Haffman::writeToFile()
{
    ofstream file("nodes.dot");
    if (file)
    {
        file << text[0].toStdString() << endl;
        file << "node [shape = none;]" << endl;
        int border = 2*step+2;
        for (int i = 1; i < border; ++i)
        {
            file << i;
            if (i != border-1)
            {
                file << "->";
            }
        }
        file << "[arrowhead=none;]" << endl;
        size_t size = text.size();
        for (size_t i = 1; i < size; ++i)
        {
            file << text[i].toStdString() << endl;
        }
        int step_i = 0;
        for (int i = 1; i < border; i+=2)
        {
            file << "{ rank=same; " << i << ";" << "node" << step_i << "0; }" << endl;
            ++step_i;
        }
        file << endl << "}";
    }
    file.close();
}

void Haffman::Haffman_alg(std::vector<Haffman::node> &node_arr)
{
    text.push_back("subgraph cluster" + QString::number(step) + "{");
    text.push_back("rank=same;");
    // выписываем вероятности из массива
    int i = node_arr.size() - 1;
    for (auto it = node_arr.begin(); it != node_arr.end(); ++it)
    {
        auto newNode = createNodeStr(QString::number(step) + QString::number(i), it->first, it->second);
        text.push_back(newNode);
        --i;
    }
    // складываем две наим. вероятности (с конца массива два элемента)
    const size_t arr_N = node_arr.size();
    // и заодно будем составлять кодовое дерево в обратном порядке (от листьев)
    // для верхней вероятности 1, так как она выше - значит больше
    const auto first_elem = node_arr[arr_N-2];
    for (auto c : first_elem.first)
    {
        haffman_map[c].append("1");
    }
    // тут соответственно 0
    const auto second_elem = node_arr[arr_N-1];
    for (auto c : second_elem.first)
    {
        haffman_map[c].append("0");
    }
    node newNode = {first_elem.first + second_elem.first, first_elem.second + second_elem.second};
    auto newNodeToStr = createNodeStr( QString::number(step) + "0" + "1",
                                    newNode.first,
                                    newNode.second  );
    text.push_back("}");
    // соединяем
    auto connectionStr = addLabel(QString::number(step)+"1",QString::number(step)+"0",QString::number(step)+"01");
    text.push_back(newNodeToStr);
    text.push_back(connectionStr);
    // получаем новый массив вероятностей (на 1 элем меньше)
    node_arr.pop_back();
    node_arr.back() = newNode;
    // сортируем вероятности
    std::sort(node_arr.begin(),node_arr.end(),[](const node &a, const node &b){
            return a.second > b.second;
    });
    // повторяем алгоритм пока не достигли вершины дерева
    if (node_arr.size() > 1)
    {
        ++step;
        Haffman_alg(node_arr);
    }
}

void Haffman::calculateL()
{
    for (auto it = node_array.begin(); it != node_array.end(); ++it)
    {
        QChar symbol = it->first[0];
        double w = double(it->second) / N;
        int len = haffman_map[symbol].length();
        L += w * len;
    }
}
