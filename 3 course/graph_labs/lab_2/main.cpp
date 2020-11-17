#include <iostream>
#include <unordered_set>
#include <random>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "graph.h"
#include "graph_im.h"
#include "graph_al.h"
#include "dfs.h"
#include "bfs.h"
#include "distancematrix.h"

using namespace std;
using namespace std::chrono;
using sec = duration<double>;
using microsec = duration<double,std::micro>;

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

    /* Тестовые данные
        Graph_AL G_AL{8,10};
        G_AL.insert({1,7});
        G_AL.insert({0,7});
        G_AL.insert({4,6});
        G_AL.insert({0,5});
        G_AL.insert({4,5});
        G_AL.insert({4,7});
        G_AL.insert({0,2});
        G_AL.insert({2,6});
        G_AL.insert({3,5});
        G_AL.insert({3,4});
    */

    // Создаем граф
    Graph_AL G_AL{V,E};
    // Генерируем
    randG(G_AL);

    // Выводим в файл представление графа
    ofstream graph_out("Graph.txt");
    if (graph_out)
    {
        graph_out << G_AL;
        graph_out.close();
    } else cout << "Can't create Graph.txt" << endl;

    // Поиск в глубину: выводим последовательности посещаемых вершин в тот же файл
    auto start = std::chrono::steady_clock::now();
    DFS dfs(G_AL);
    auto end = std::chrono::steady_clock::now();
    auto res = duration_cast<microsec>(end - start);
    cout << "DFS run time: " << res.count() << " microseconds" << endl;
    ofstream dfs_out("Graph.txt", ios_base::app);
    if (dfs_out)
    {
        dfs_out << endl << dfs;
        dfs_out.close();
    } else cout << "Can't open Graph.txt" << endl;

    // Преобразование из списка смежных вершин в матрицу инцидентности
    Graph_IM G_IM(G_AL);
    // Вывод преобразования в файл
    ofstream graph1_out("Graph1.txt");
    if (graph1_out)
    {
        graph1_out << G_IM;
        graph1_out.close();
    } else cout << "Can't create Graph1.txt" << endl;

    // Поиск в ширину: выводим последовательности посещаемых вершин в тот же файл
    start = std::chrono::steady_clock::now();
    BFS bfs(G_IM);
    end = std::chrono::steady_clock::now();
    res = duration_cast<microsec>(end - start);
    cout << "BFS run time: " << res.count() << " microseconds" << endl;
    ofstream bfs_out("Graph1.txt", ios_base::app);
    if (bfs_out)
    {
        bfs_out << endl << bfs;
        bfs_out.close();
    } else cout << "Can't open Graph1.txt" << endl;

    // Высчитываем диаметр графа
    DistanceMatrix DM(G_IM);
    cout << DM;
    cout << "Diameter: " << DM.getDiameter() << endl;
    cout << "End";
    return 0;
}
