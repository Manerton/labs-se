#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <queue>
#include <tuple>
using namespace std;

using key_t = uint64_t;

class BTree
{
public:
    using value_t = size_t; // храним индексы в качестве значения
    using data_t = std::tuple<key_t,value_t>;
private:

    // Узел нашего дерева
    struct node_t
    {
    // ПОЛЯ ----------
        data_t *elements;        // Массив ключей и индексов
        int t;              // Минимальная степень (определяет диапазон для количества ключей)
        node_t **C;         // Массив дочерних указателей
        bool leaf;          // Истинно, когда узел является листом. В противном случае ложь
        int n = 0;          // Текущее количество ключей
        node_t(int _t, bool _leaf);   // Конструктор
    // МЕТОДЫ --------
        // получения ключа и индекса
        key_t& key(int i) {return std::get<0>(elements[i]);}
        value_t& value(int i) {return std::get<1>(elements[i]);}
        key_t key(int i) const {return std::get<0>(elements[i]);}
        value_t value(int i) const {return std::get<1>(elements[i]);}
        // Функции обхода для прохождения всех узлов
        // InOrder (как бы в порядке возрастания)
        void traverse();
        // Обход в ширину (для проверки правильности построения структуры дерева)
        std::string traverseIterative();
        // Функция для поиска ключа в поддереве с корнем из этого узла.
        node_t* search(key_t k);   // возвращает nullptr, если k отсутствует.
        // Функция, которая возвращает индекс первого ключа, который >= k
        int findKey(key_t k);
        // вставка ключа k в дерево с незаполненным корневым узлом
        void insertNonFull(const data_t &elem);
        // вспомогательная функция для разделения на две части потомка "y" этого (this) узла
        void splitChild(int i, node_t *y);
        // функция-обертка для удаления ключа k
        bool remove(key_t k);
        // функция для удаления ключа, присутствующего в idx-й позиции в узле, являющемся листом
        void removeFromLeaf(int idx);
        // функция для удаления ключа, присутствующего в idx-й позиции в узле, не являющемся листом
        void removeFromNonLeaf(int idx);
        // функция для получения ключа - предшественника для другого ключа
        data_t getPred(int idx);
        // функция для получения ключа, который следует за другим ключом в узле
        data_t getSled(int idx);
        // функция для заполнения дочернего C[idx], которая имеет менее чем t-1 ключей
        // поскольку иначе из C[idx] нельзя удалить ключ
        void fill(int idx);
        // функция для заимствования ключа из C[idx-1]
        void borrowFromPrev(int idx);
        // Функция для заимствования ключа из C [idx+1]
        void borrowFromNext(int idx);
        // функция для объединения двух узлов, похоже на расщепление, но в обратную сторону
        void merge(int idx);
    };
// ПОЛЯ
    node_t *root = nullptr; // корень
    int t;  // Минимальная степень
// МЕТОДЫ
// проверка на уникальный ключ
    bool check_for_unique_key(const key_t& key);
public:
    // Конструктор (инициализирует дерево как пустое)
    BTree(int _t) : t(_t) {}
    BTree() : t(2) {}
    void traverse()
    {
        if (root != nullptr) root->traverse();
    }
    std::string traverseIterative()
    {
        if (root != nullptr) return root->traverseIterative();
        return std::string("");
    }
    // получения ключа и индекса из data_t
    static key_t get_key(const data_t &elem) noexcept {return std::get<0>(elem);};
    static value_t get_value(const data_t &elem) noexcept {return std::get<1>(elem);};
    // функция для поиска ключа в этом дереве
    node_t* search(key_t k) const
    {
        if (root != nullptr) return root->search(k);
        else return nullptr;
    }
    size_t find(key_t k) const
    {
        auto res = search(k);
        if (res != nullptr)
        {
            int i = 0;
            while (i < res->n && k != res->key(i)) ++i;
            return res->value(i);
        }
        return 0;
    }
    // вставляем новый ключ в B-дерево
    bool insert(const data_t &elem);
    bool replace(const data_t &elem);
    // удаляем ключ в B-Tree
    bool remove(key_t k);
};

#endif // BTREE_H
