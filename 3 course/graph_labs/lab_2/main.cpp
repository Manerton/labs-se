#include <iostream>
#include <unordered_set>
#include <random>
#include <ctime>
#include <fstream>
#include "graph.h"
#include "graph_im.h"
#include "graph_al.h"
#include "degree.h"

using namespace std;

static mt19937 mt(time(0));

// генерирование случайного связного неориентированного графа без петель и кратных рёбер
void randG(Graph &G)
{
    using size_type = Graph::size_type;
    // количество вершин графа
    const size_type V = G.getV();
    // количество ребер графа
    const size_type E = G.getE();

    // хеш-таблица для быстрого отсеивания повторяющихся сгенерированных ребер
    // будем помещать в таблицу ключи по формуле: первая вершина v * offset + вторая вершина w
    // поскольку по условию задачи количество вершин генерируется от 20 до 30, то offset = 100 будет достаточно
    // например: v = 15, w = 18, offset = 100, key = 1518
    std::unordered_set<uint16_t> hashtable;
    const uint8_t offset = 100;

    // указываем диапазон от 0 до V-1 для генерирования случайных ребер

    uniform_int_distribution<int> r_dist(0, V-1);

    // первые V-1 ребер сгенерируем для гарантированной связности графа по следующему алгоритму
    // сформируем массив вершин по порядку от 0 до V-1
    vector<size_type> v_arr(V);
    iota(v_arr.begin(),v_arr.end(),0);
    // перемешаем
    shuffle(v_arr.begin(),v_arr.end(),mt);
    // добавим в граф
    for (size_type k = 0; k < V-1; ++k)
    {
        size_type v = v_arr[k];
        size_type w = v_arr[k+1];
        if (v > w) std::swap(v,w);

        uint16_t key = v * offset + w;
        hashtable.insert(key);
        G.insert({v,w});
    }
    // оставшиеся ребра добавляем с проверкой на петли и использованием хеш-таблицы (для отсеивания кратных ребер)
    size_type i = V-1;
    while (i < E)
    {
        int v = r_dist(mt);
        int w = r_dist(mt);
        if (v > w) std::swap(v,w);

        uint16_t key = v * offset + w;
        Graph::Edge e = {v,w};

        if (v != w && (hashtable.find(key) == hashtable.end()))
        {
            hashtable.insert(key);
            G.insert(e);
            ++i;
        }
    }
}

int main()
{
    // Генерируем кол-во вершин и ребер
    uniform_int_distribution<int> r_v(20, 30);
    Graph::size_type V = r_v(mt);
    uniform_int_distribution<int> r_e(V-1, (V*(V-1)/2));
    Graph::size_type E = r_e(mt);
    cout << V << " " << E << endl;
    // Создаем граф
    Graph_AL G_AL{5,8};

    // Генерируем
    randG(G_AL);

    // Выводим в файл
    ofstream graph_out("Graph.txt");
    if (graph_out)
    {
        graph_out << G_AL;
        graph_out.close();
    } else cout << "Can't create Graph.txt" << endl;

    // Вычисляем степени вершин графа и выводим в файл
    Degree D1(G_AL);
    ofstream degree_out("Degree.txt");
    if (degree_out)
    {
        degree_out << D1;
        degree_out.close();
    } else cout << "Can't create Degree.txt" << endl;

 /*   // Преобразование 1
    Graph_EL list(G_IM);
    // Вывод преобразования в файл
    ofstream graph1_out("Graph1.txt");
    if (graph1_out)
    {
        graph1_out << list;
        graph1_out.close();
    } else cout << "Can't create Graph1.txt" << endl;

    // Вычисляем степени вершин преобразования 1 и сравниваем с файлом Degree.txt
    Degree D2(list);
    ifstream input("Degree.txt");
    if (input)
    {
        int V = list.getV();
        Graph::size_type fromFile = 0;
        for (int i = 0; i < V; ++i)
        {
            input >> fromFile;
            assert(fromFile == D2[i]);
        }
        input.close();
    }

    // Преобразование 2
    Graph_AM G_AM(list);
    ofstream graph2_out("Graph2.txt");
    if (graph2_out)
    {
        graph2_out << G_AM;
        graph2_out.close();
    } else cout << "Can't create Graph2.txt" << endl;

    // Вычисляем степени вершин преобразования 2 и сравниваем с файлом Degree.txt
    Degree D3(G_AM);
    input.clear();
    input.seekg(0, ios::beg);
    input.open("Degree.txt");
    if (input)
    {
        int V = G_AM.getV();
        Graph::size_type fromFile = 0;
        for (int i = 0; i < V; ++i)
        {
            input >> fromFile;
            assert(fromFile == D3[i]);
        }
        input.close();
    }*/
    cout << "End";
    return 0;
}
