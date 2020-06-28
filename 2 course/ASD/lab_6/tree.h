#ifndef TREE_H
#define TREE_H
#include <cstdint>
#include <tuple>
#include <exception>

class AVLtree
{
public:
    // ID - идентификационный номер очередника
    using key_t   = uint64_t;
    // данные элемента контейнера
    using value_t = std::tuple<
        uint8_t,   // Family_member_count
        uint16_t,   // Year
        uint8_t,    // Month
        float,      // Area_occupied
        uint8_t,    // Rooms_count
        float       // Area_required
        >; // -- квартирант/очередник -- //
    // ключ (ID) + данные
    using data_t  = std::tuple<key_t, value_t>;
private:
    struct node_t
    {
        // данные + ключ узла
        data_t element;
    // высота
        int8_t _height = 0;
    // указатели на левого и правого потомка
        node_t *left    = nullptr;
        node_t *right   = nullptr;
    // конструктор
        node_t(const data_t &tmp) : element(tmp), _height(1) {};
        ~node_t()
        {
            left = right = nullptr;
        }
    };
    node_t *root = nullptr; // корень дерева
    size_t _count = 0;        // количество элементов в дереве

// полное рекурсивное удаление дерева
    void erase_all(node_t *&node);
// рекурсивное добавление узла в дерево
    void add_node(const key_t& key, const value_t& v, node_t* &node);
// рекурсивные обходы
    void DoPreOrderRecurse(std::ofstream &write, node_t *node) const;
    void DoInOrderRecurse(std::ofstream &write, node_t *node) const;
    void DoPostOrderRecurse(std::ofstream &write, node_t *node) const;
// рекурсивный поиск нода
    node_t* findNode(const key_t& key, node_t *node);
// проверка на уникальный ключ
    bool check_for_unique_key(const key_t& key);
// для функции копирования / объединения двух деревьев
    void PreOrderCopy(node_t *node);
// поиск минимума и максимума
    node_t* find_min_node(node_t* node);
    node_t* find_max_node(node_t* node);
// удаление узла с минимальным ключом из дерева p
    node_t* remove_min_node(node_t* p);
// рекурсивное удаление нода
    node_t* removeNode(const key_t &key, node_t *p);
// вычисляем balance factor заданного узла
    int8_t bFactor(const node_t* node) const;
// восстановление корректного значения поля height заданного узла
    void fixHeight(node_t* node);
// поиск высоты
    int8_t find_height(AVLtree::node_t *node) const noexcept;
public:
    AVLtree(void) = default;
    virtual ~AVLtree(void);
    AVLtree(const data_t& d) noexcept;
    AVLtree(const std::initializer_list<data_t> &t);
    AVLtree(const AVLtree& other);
    AVLtree(AVLtree&& other);                // -- конструктор переноса --
    AVLtree& operator=(AVLtree&& other);     // -- операция перемещания --
    AVLtree& operator=(const AVLtree& other);
// Получение значений
    static key_t get_key(const data_t &elem) noexcept;
    static value_t get_value(const data_t &elem) noexcept;
    data_t get_root_data();
    key_t get_root_key();
// Количества ------------------
    bool empty() const noexcept;
    size_t count() const noexcept;              // -- количенство элементов --
    int8_t height(node_t* node) const noexcept;             // -- высота узла поддерева --
    int8_t height() const noexcept;             // -- высота дерева --
// Повороты ---------------------
    node_t* rightRotate(node_t* p) noexcept;
    node_t* leftRotate(node_t* q) noexcept;
// Балансировка -----------------
    node_t* balance(node_t* p) noexcept;
// Поиск ------------------------
    data_t find(const key_t& key);
    data_t find_min();
    data_t find_max();
// Модификаторы контейнера ------
// Вставить --
    bool insert(const key_t& key, const value_t& v);
    bool insert(const data_t& d);
// Заменить --
    bool replace(const key_t& key, const value_t& v);
// Удалить --
    bool erase (const key_t& key);              // удалить указанный
    void clear ();                              // удалить все
// обмен --
    void swap (AVLtree &t) noexcept;        // обменять с заданным списком
// Обходы дерева ----------------
// обход дерева - рекурсивный
    void PreOrderRecurse(const std::string &filename) const noexcept;
    void InOrderRecurse(const std::string &filename) const noexcept;
    void PostOrderRecurse(const std::string &filename) const noexcept;
// обход дерева - итеративный
    void PreOrderIterative(const std::string &filename) const noexcept;
    void InOrderIterative(const std::string &filename) const noexcept;
    void PostOrderIterative(const std::string &filename) const noexcept;
// Объединение двух деревьев ----
    void concat(const AVLtree &b);
};
// вывод элемента в строку
std::string elemToString(const AVLtree::data_t &element);

#endif // TREE_H
